#include <Arduino.h>
#include <Keyboard.h>

// —— CONFIG ——
#define DEBUG_MODE false

// —— MATRIX DIMENSIONS ——
const uint8_t ROWS = 6;
const uint8_t COLS = 21;

// —— COLUMN PINS ——
const uint8_t colPins[COLS] = {
   1,  2,  3,  4,  5,  6,  7,
   8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21
};

// —— SN74HC165 PINS ——
#define SN_LOAD_PIN  0
#define SN_CLK_PIN   26
#define SN_DATA_PIN  27

// —— KEYMAP ——
const char* keymap[ROWS][COLS] = {
  {"ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "____", "PSCR", "SLCK", "PAUS", "NUM", "NP_/", "NP_*", "NP_-"},
  {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BSPC", "INS", "HOME", "PGUP", "NP_7", "NP_8", "NP_9", "NP_+"},
  {"TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\", "DEL", "END", "PGDN", "NP_4", "NP_5", "NP_6", "____"},
  {"CAPS", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "____", "ENTER", "____", "UP", "____", "NP_1", "NP_2", "NP_3", "NP_ENTER"},
  {"LSHFT", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "____", "____", "RSHFT", "____", "LEFT", "DOWN", "RIGHT", "NP_0", "____", "NP_."},
  {"LCTRL", "LGUI", "LALT", "____", "____", "SPACE", "____", "____", "RALT", "MENU", "RGUI", "____", "____", "RCTRL", "____", "____", "____", "____", "____", "____"}
};

// —— SHIFTED SYMBOLS ——
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

// —— Key Label → KeyCode ——
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

    // Handle letters
    if (ch >= 'a' && ch <= 'z') {
      if ((capsLockActive && !shiftActive) || (!capsLockActive && shiftActive)) {
        useShift = true;
      }
      return tolower(ch);
    }

    // Handle shifted symbols
    char shifted = getShiftedSymbol(ch);
    if (shifted != 0 && shiftActive) {
      useShift = true;
      return shifted;
    }

    return ch;
  }

  return 0;
}

// —— Read SN74HC165 ——
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

// —— Setup ——
void setup() {
  Serial.begin(115200);
  Keyboard.begin();

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

// —— Main Loop ——
void loop() {
  bool rowDetected[8] = {false};

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
        }
      }
    }
  }

  delay(10);
}
