#include "matrix.h"
#include "wait.h"
#include "debug.h"
#include "timer.h"
#include <string.h>
#include "gpio.h"  // QMK platform-specific GPIO control

// Define SN74HC165 control pins (update if needed)
#define SHIFT_PL        GP0    // Parallel load (active low)
#define SHIFT_CLK       GP26   // Clock
#define SHIFT_QH        GP27   // Serial data output

// Define matrix dimensions
#define NUM_ROWS 6
const pin_t MATRIX_COLS_PINS[] = {
    GP1, GP2, GP3, GP4, GP5, GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13, GP14, GP15, GP16, GP17, GP18, GP19, GP20, GP21
};
#define NUM_COLS (sizeof(MATRIX_COLS_PINS) / sizeof(MATRIX_COLS_PINS[0]))

static uint32_t last_debug_time = 0;

// Initialize GPIO pins for shift register and columns
void matrix_init_custom(void) {
    // Initialize SN74HC165 pins
    gpio_set_pin_output(SHIFT_PL);
    gpio_write_pin_high(SHIFT_PL);

    gpio_set_pin_output(SHIFT_CLK);
    gpio_write_pin_low(SHIFT_CLK);

    gpio_set_pin_input(SHIFT_QH);

    // Initialize all column pins
    for (int i = 0; i < NUM_COLS; i++) {
        gpio_set_pin_output(MATRIX_COLS_PINS[i]);
        gpio_write_pin_low(MATRIX_COLS_PINS[i]);
    }
}

// Read 6 bits from the shift register
uint8_t sn74hc165_read_rows(void) {
    uint8_t data = 0;

    // Latch inputs
    gpio_write_pin_low(SHIFT_PL);
    wait_us(5);
    gpio_write_pin_high(SHIFT_PL);
    wait_us(1);

    for (int i = 0; i < NUM_ROWS; i++) {
        data <<= 1;
        if (gpio_read_pin(SHIFT_QH)) {
            data |= 1;
        }

        gpio_write_pin_high(SHIFT_CLK);
        wait_us(1);
        gpio_write_pin_low(SHIFT_CLK);
        wait_us(1);
    }

    return data;
}

// Main scan function
bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool updated = false;
    bool debug = false;

    if (timer_elapsed(last_debug_time) > 500) {
        debug = true;
        last_debug_time = timer_read();
    }

    matrix_row_t scanned_matrix[NUM_ROWS] = {0};

    // Activate each column and scan
    for (int col = 0; col < NUM_COLS; col++) {
        for (int c = 0; c < NUM_COLS; c++) {
            gpio_write_pin(MATRIX_COLS_PINS[c], c == col);
        }

        wait_us(100);

        uint8_t row_bits = sn74hc165_read_rows();

        if (debug) {
            dprintf("Col %d: row bits = 0x%02X\n", col, row_bits);
        }

        for (int row = 0; row < NUM_ROWS; row++) {
            if (row_bits & (1 << (NUM_ROWS - 1 - row))) {
                scanned_matrix[row] |= (1 << col);
            }
        }
    }

    for (int row = 0; row < NUM_ROWS; row++) {
        if (current_matrix[row] != scanned_matrix[row]) {
            current_matrix[row] = scanned_matrix[row];
            updated = true;
        }
    }

    return updated;
}
