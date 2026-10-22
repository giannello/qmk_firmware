// Copyright 2026 Giuseppe Iannello (@giannello)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "matrix.h"
#include "atomic_util.h"
#include <string.h>

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

#define MATRIX_SCAN_ROWS 8
#define EXTRA_ROW 8
#define EXTRA_KEY_COUNT 5

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static const pin_t extra_pins[EXTRA_KEY_COUNT] = {
    D0, D1, A12, A13, A14
};

static inline void select_col_pin(pin_t pin) {
    ATOMIC_BLOCK_FORCEON { gpio_set_pin_output(pin); gpio_write_pin_low(pin); }
}
static inline void unselect_col_pin(pin_t pin) {
    ATOMIC_BLOCK_FORCEON { gpio_set_pin_input_high(pin); }
}
static void init_pins(void) {
    for (uint8_t row = 0; row < MATRIX_SCAN_ROWS; row++) {
        gpio_set_pin_input_high(row_pins[row]);
    }
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        gpio_set_pin_input_high(col_pins[col]);
    }
    for (uint8_t i = 0; i < EXTRA_KEY_COUNT; i++) {
        gpio_set_pin_input_high(extra_pins[i]);
    }
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool changed = false;
    select_col_pin(col_pins[current_col]);
    matrix_output_select_delay();
    for (uint8_t row = 0; row < MATRIX_SCAN_ROWS; row++) {
        matrix_row_t last_row_value = current_matrix[row];
        matrix_row_t current_row_value = last_row_value;
        if (MATRIX_INPUT_PRESSED_STATE == gpio_read_pin(row_pins[row])) {
            current_row_value |= (MATRIX_ROW_SHIFTER << current_col);
        } else {
            current_row_value &= ~(MATRIX_ROW_SHIFTER << current_col);
        }
        if (last_row_value != current_row_value) {
            current_matrix[row] = current_row_value;
            changed = true;
        }
    }
    unselect_col_pin(col_pins[current_col]);
    matrix_output_unselect_delay(current_col, changed);
    return changed;
}

void matrix_init_custom(void) {
    init_pins();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        changed |= read_rows_on_col(current_matrix, col);
    }
    matrix_row_t last_extra = current_matrix[EXTRA_ROW];
    matrix_row_t extra = 0;
    for (uint8_t i = 0; i < EXTRA_KEY_COUNT; i++) {
        if (gpio_read_pin(extra_pins[i]) == MATRIX_INPUT_PRESSED_STATE) {
            extra |= (MATRIX_ROW_SHIFTER << i);
        }
    }
    if (last_extra != extra) {
        current_matrix[EXTRA_ROW] = extra;
        changed = true;
    }
    return changed;
}
