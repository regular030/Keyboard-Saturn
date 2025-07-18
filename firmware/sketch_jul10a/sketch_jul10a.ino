#include <SoftI2C.h>
#include <Arduino.h>
#include <Keyboard.h>
#include "font5x7.h"  // Your separate font header with font5x7 array

#define SDA_PIN 22
#define SCL_PIN 28

SoftI2C SoftWire(SDA_PIN, SCL_PIN);

#define OLED_ADDR 0x3C

// OLED command and data functions (same as before)
void oled_command(uint8_t cmd) {
  SoftWire.beginTransmission(OLED_ADDR);
  SoftWire.write(0x00); // command mode
  SoftWire.write(cmd);
  SoftWire.endTransmission();
}

void oled_data(uint8_t *data, size_t len) {
  SoftWire.beginTransmission(OLED_ADDR);
  SoftWire.write(0x40); // data mode
  for (size_t i=0; i<len; i++) SoftWire.write(data[i]);
  SoftWire.endTransmission();
}

void oled_init() {
  oled_command(0xAE); oled_command(0xD5); oled_command(0x80);
  oled_command(0xA8); oled_command(0x1F);
  oled_command(0xD3); oled_command(0x00);
  oled_command(0x40);
  oled_command(0x8D); oled_command(0x14);
  oled_command(0x20); oled_command(0x00);
  oled_command(0xA1); oled_command(0xC8);
  oled_command(0xDA); oled_command(0x02);
  oled_command(0x81); oled_command(0x8F);
  oled_command(0xD9); oled_command(0xF1);
  oled_command(0xDB); oled_command(0x40);
  oled_command(0xA4); oled_command(0xA6);
  oled_command(0xAF);
}

void oled_clear() {
  uint8_t zero[128] = {0};
  for (uint8_t page=0; page<4; page++) {
    oled_command(0xB0 + page);
    oled_command(0x00);
    oled_command(0x10);
    oled_data(zero, 128);
  }
}

// Draw one character at (page, col)
void oled_draw_char(char c, uint8_t page, uint8_t col) {
  if (c < 32 || c > 126) c = '?';
  const uint8_t* bitmap = font5x7[c - 32];
  oled_command(0xB0 + page);
  oled_command(0x00 + (col & 0x0F));
  oled_command(0x10 + ((col >> 4) & 0x0F));
  uint8_t buf[6];
  memcpy(buf, bitmap, 5);
  buf[5] = 0x00;
  oled_data(buf, 6);
}

// Draw string starting at page and column
void oled_draw_string(const char* str, uint8_t page, uint8_t col) {
  while (*str && col < 128 - 6) {
    oled_draw_char(*str++, page, col);
    col += 6;
  }
}

// Keyboard matrix definitions
#define DEBUG_MODE false

const uint8_t ROWS = 6;
const uint8_t COLS = 21;

const uint8_t colPins[COLS] = {
   1,  2,  3,  4,  5,  6,  7,
   8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21
};

#define SN_LOAD_PIN  0
#define SN_CLK_PIN   26
#define SN_DATA_PIN  27

const char* keymap[ROWS][COLS] = {
  {"ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "____", "PSCR", "SLCK", "PAUS", "NUM", "NP_/", "NP_*", "NP_-"},
  {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BSPC", "INS", "HOME", "PGUP", "NP_7", "NP_8", "NP_9", "NP_+"},
  {"TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\", "DEL", "END", "PGDN", "NP_4", "NP_5", "NP_6", "____"},
  {"CAPS", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "____", "ENTER", "____", "UP", "____", "NP_1", "NP_2", "NP_3", "NP_ENTER"},
  {"LSHFT", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "____", "____", "RSHFT", "____", "LEFT", "DOWN", "RIGHT", "NP_0", "____", "NP_."},
  {"LCTRL", "LGUI", "LALT", "____", "____", "SPACE", "____", "____", "RALT", "MENU", "RGUI", "____", "____", "RCTRL", "____", "____", "____", "____", "____", "____"}
};

char getShiftedSymbol(char key) {
  switch (key) {
    case '1': return '!';
    case '2': return '@';
    case '3': return '#';
    case '4': return '$';
    case '5': return '%';
    case '6': return '^';
    case '7': return '&';
    case '8': return '*';
    case '9': return '(';
    case '0': return ')';
    case '-': return '_';
    case '=': return '+';
    case '[': return '{';
    case ']': return '}';
    case '\\': return '|';
    case ';': return ':';
    case '\'': return '"';
    case ',': return '<';
    case '.': return '>';
    case '/': return '?';
    case '`': return '~';
    default: return 0;
  }
}

bool capsLockActive = false;
bool shiftActive = false;

uint8_t getKeyCode(const char* label, bool& useShift) {
  useShift = false;

  if (strcmp(label, "ENTER") == 0) return KEY_RETURN;
  if (strcmp(label, "ESC") == 0) return KEY_ESC;
  if (strcmp(label, "BSPC") == 0) return KEY_BACKSPACE;
  if (strcmp(label, "TAB") == 0) return KEY_TAB;
  if (strcmp(label, "SPACE") == 0) return ' ';
  if (strcmp(label, "DEL") == 0) return KEY_DELETE;
  if (strcmp(label, "UP") == 0) return KEY_UP_ARROW;
  if (strcmp(label, "DOWN") == 0) return KEY_DOWN_ARROW;
  if (strcmp(label, "LEFT") == 0) return KEY_LEFT_ARROW;
  if (strcmp(label, "RIGHT") == 0) return KEY_RIGHT_ARROW;

  if (strlen(label) == 1) {
    char ch = label[0];

    if (ch >= 'a' && ch <= 'z') {
      if ((capsLockActive && !shiftActive) || (!capsLockActive && shiftActive)) {
        useShift = true;
      }
      return tolower(ch);
    }

    char shifted = getShiftedSymbol(ch);
    if (shifted != 0 && shiftActive) {
      useShift = true;
      return shifted;
    }

    return ch;
  }

  return 0;
}

uint8_t readRows() {
  digitalWrite(SN_LOAD_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(SN_LOAD_PIN, HIGH);
  delayMicroseconds(3);

  uint8_t data = 0;
  for (int i = 0; i < 8; i++) {
    data <<= 1;
    digitalWrite(SN_CLK_PIN, LOW);
    delayMicroseconds(1);
    data |= digitalRead(SN_DATA_PIN);
    digitalWrite(SN_CLK_PIN, HIGH);
    delayMicroseconds(1);
  }

  return data;
}

void setup() {
  Serial.begin(115200);
  Keyboard.begin();

  SoftWire.begin();

  oled_init();
  oled_clear();
  oled_draw_string("Ready...", 1, 0);

  for (uint8_t c = 0; c < COLS; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], LOW);
  }

  pinMode(SN_LOAD_PIN, OUTPUT);
  pinMode(SN_CLK_PIN, OUTPUT);
  pinMode(SN_DATA_PIN, INPUT);

  digitalWrite(SN_LOAD_PIN, HIGH);
  digitalWrite(SN_CLK_PIN, HIGH);

  Serial.println("Matrix scanner initialized.");
}

void loop() {
  static bool rowDetected[8] = {false};

  for (uint8_t col = 0; col < COLS; col++) {
    for (uint8_t c = 0; c < COLS; c++) digitalWrite(colPins[c], LOW);
    digitalWrite(colPins[col], HIGH);
    delay(30);

    digitalWrite(SN_LOAD_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(SN_LOAD_PIN, HIGH);
    delayMicroseconds(5);

    uint8_t rowBits = readRows();
    digitalWrite(colPins[col], LOW);

    for (uint8_t bit = 0; bit < ROWS; bit++) {
      bool isPressed = (rowBits >> bit) & 1;
      if (isPressed && !rowDetected[bit]) {
        rowDetected[bit] = true;

        const char* label = keymap[bit][col];
        if (!label || strcmp(label, "____") == 0) continue;

        if (DEBUG_MODE) {
          Serial.println(label);
        } else {
          if (strcmp(label, "CAPS") == 0) {
            capsLockActive = !capsLockActive;
            Serial.println(capsLockActive ? "CAPS ON" : "CAPS OFF");
            continue;
          }

          if (strcmp(label, "LSHFT") == 0 || strcmp(label, "RSHFT") == 0) {
            shiftActive = true;
            continue;
          }

          bool useShift = false;
          uint8_t keycode = getKeyCode(label, useShift);

          if (keycode != 0) {
            if (useShift) Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press(keycode);
            delay(5);
            Keyboard.release(keycode);
            if (useShift) Keyboard.release(KEY_LEFT_SHIFT);
          }

          shiftActive = false;

          // Display pressed key on OLED
          oled_clear();
          oled_draw_string(label, 1, 0);
        }
      }
      else if (!isPressed) {
        // Reset detection for that row so next press can be detected
        rowDetected[bit] = false;
      }
    }
  }

  delay(10);
}
