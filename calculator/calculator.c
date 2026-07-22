// Copyright 2026 azar
// SPDX-License-Identifier: GPL-2.0-or-later
#include "quantum.h"
#include <limits.h>
#include <stdio.h>

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 1, 0);

// --- User-configurable, set these in the keymap's own config.h ---
#ifndef CALCULATOR_BIT_LEDS
#    define CALCULATOR_BIT_LEDS { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
#endif
#ifndef CALCULATOR_SIGN_LED
#    define CALCULATOR_SIGN_LED 255 // 255 = none
#endif
#ifndef CALCULATOR_NUMLOCK_LED
#    define CALCULATOR_NUMLOCK_LED 255 // 255 = none
#endif

static const uint8_t calc_leds[]     = CALCULATOR_BIT_LEDS;
static const uint8_t calc_nleds      = ARRAY_SIZE(calc_leds);
static const uint8_t calc_sign_led   = CALCULATOR_SIGN_LED;
static const uint8_t calc_numlock_led = CALCULATOR_NUMLOCK_LED;

static bool active       = false;
static long accumulator  = 0;
static long current_num  = 0;
static bool negative     = false;
static char pending_op   = 0;
static bool show_result  = false;
static bool overflow     = false;

static void calculator_reset(void) {
    accumulator = 0;
    current_num = 0;
    negative    = false;
    pending_op  = 0;
    show_result = false;
    overflow    = false;
}

static void apply_pending(void) {
    long value = negative ? -current_num : current_num;
    switch (pending_op) {
        case 0:   accumulator = value; break;
        case '+': accumulator += value; break;
        case '-': accumulator -= value; break;
        case '*': accumulator *= value; break;
        case '/':
            if (value == 0) overflow = true;
            else accumulator /= value;
            break;
    }
    current_num = 0;
    negative    = false;
}

static void send_decimal(long value) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", value);
    SEND_STRING(buf);
}

static void send_binary(unsigned long uval) {
    char buf[34];
    uint8_t i = 0;
    if (uval == 0) {
        buf[i++] = '0';
    } else {
        char tmp[32];
        uint8_t n = 0;
        while (uval > 0) {
            tmp[n++] = (uval & 1) ? '1' : '0';
            uval >>= 1;
        }
        while (n > 0) buf[i++] = tmp[--n];
    }
    buf[i] = '\0';
    SEND_STRING(buf);
}

bool process_record_calculator(uint16_t keycode, keyrecord_t *record) {
    if (keycode == CALC_TOGGLE) {
        if (record->event.pressed) {
            active = !active;
            calculator_reset();
        }
        return false;
    }

    if (!active) return true; // let kb/user hooks handle everything else

    if (!record->event.pressed) {
        switch (keycode) {
            case KC_P1 ... KC_P0:
            case KC_PPLS: case KC_PMNS: case KC_PAST: case KC_PSLS:
            case KC_PEQL: case KC_PENT: case KC_PDOT: case KC_BSPC:
            case KC_V: case KC_B:
                return false;
            default:
                return true;
        }
    }

    switch (keycode) {
        case KC_P1 ... KC_P0: {
            uint8_t digit = (keycode == KC_P0) ? 0 : (keycode - KC_P1 + 1);
            if (show_result) calculator_reset();
            if (current_num <= (LONG_MAX - digit) / 10) {
                current_num = current_num * 10 + digit;
            } else {
                overflow = true;
            }
            return false;
        }

        case KC_PDOT:
            negative = !negative;
            return false;

        case KC_BSPC:
            current_num /= 10;
            return false;

        case KC_PPLS: case KC_PMNS: case KC_PAST: case KC_PSLS:
            apply_pending();
            pending_op  = (keycode == KC_PPLS) ? '+' :
                          (keycode == KC_PMNS) ? '-' :
                          (keycode == KC_PAST) ? '*' : '/';
            show_result = false;
            return false;

        case KC_PEQL: case KC_PENT:
            apply_pending();
            pending_op  = 0;
            show_result = true;
            return false;

        case KC_V: {
            long value = show_result ? accumulator : (negative ? -current_num : current_num);
            send_decimal(value);
            return false;
        }

        case KC_B: {
            long value = show_result ? accumulator : (negative ? -current_num : current_num);
            unsigned long uval = (value < 0) ? (unsigned long)(-value) : (unsigned long)value;
            send_binary(uval);
            return false;
        }

        default:
            return true;
    }
}

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_calculator(void) {
    if (!active) return true;

    rgb_matrix_set_color_all(RGB_OFF);

    bool blink = (timer_read() / 250) % 2;

    if (calc_numlock_led != 255 && blink) {
        rgb_matrix_set_color(calc_numlock_led, 255, 255, 0);
    }

    long value = show_result ? accumulator : (negative ? -current_num : current_num);
    bool neg = value < 0;
    unsigned long uval = neg ? (unsigned long)(-value) : (unsigned long)value;

    if (overflow || uval >= (1UL << calc_nleds)) {
        if (blink) {
            for (uint8_t i = 0; i < calc_nleds; i++) rgb_matrix_set_color(calc_leds[i], RGB_RED);
        }
        return false;
    }

    for (uint8_t i = 0; i < calc_nleds; i++) {
        if ((uval >> i) & 1) {
            rgb_matrix_set_color(calc_leds[i], 0, show_result ? 120 : 255, show_result ? 255 : 0);
        }
    }

    if (calc_sign_led != 255 && neg) {
        rgb_matrix_set_color(calc_sign_led, RGB_RED);
    }

    return false;
}
#endif