#include "enter_bspc.h"

typedef enum { MODE_NONE, MODE_ENTER, MODE_BACKSPACE, MODE_WORD } enter_bspc_mode_t;

static enter_bspc_mode_t current_mode          = MODE_NONE;
static uint16_t          mode_last_action_time = 0;
static bool               key_down             = false;

// Only armed when a press begins in NONE mode -- tracks the single
// continuous hold that may escalate None -> Backspace -> Word.
static deferred_token to_backspace_token = 0;
static deferred_token to_word_token      = 0;

static void refresh_mode_timer(void) {
    mode_last_action_time = timer_read();
}

static uint32_t escalate_to_word_cb(uint32_t trigger_time, void *cb_arg);

// Fires if the original press is still held ENTER_BSPC_HOLD_TERM ms later.
static uint32_t escalate_to_backspace_cb(uint32_t trigger_time, void *cb_arg) {
    // Cancel any pending Enter repeats
    if (to_backspace_token) {
        cancel_deferred_exec(to_backspace_token);
        to_backspace_token = 0;
    }
    
    unregister_code(KC_ENT);
    register_code(KC_BSPC);
    current_mode = MODE_BACKSPACE;
    refresh_mode_timer();
    
    // Still watching the SAME continuous hold for further escalation.
    to_word_token = defer_exec(ENTER_BSPC_WORD_HOLD_TERM, escalate_to_word_cb, NULL);
    return 0;
}

// Fires if that same press is still held ENTER_BSPC_WORD_HOLD_TERM ms further.
static uint32_t escalate_to_word_cb(uint32_t trigger_time, void *cb_arg) {
    unregister_code(KC_BSPC);
    register_code(KC_LCTL);
    register_code(KC_BSPC);
    current_mode = MODE_WORD;
    refresh_mode_timer();
    to_word_token = 0;
    return 0;
}

bool process_record_enter_bspc(uint16_t keycode, keyrecord_t *record) {
    if (keycode != ENT_BS) {
        return true;
    }

    if (record->event.pressed) {
        key_down = true;

        switch (current_mode) {
            case MODE_NONE:
                // No action yet - we wait to see if it's a tap or hold
                // Arm the escalation chain from NONE mode
                to_backspace_token = defer_exec(ENTER_BSPC_HOLD_TERM, escalate_to_backspace_cb, NULL);
                break;

            case MODE_ENTER:
                register_code(KC_ENT);
                // No escalation timer in Enter mode - holding repeats Enter
                break;

            case MODE_BACKSPACE:
                register_code(KC_BSPC);
                break;

            case MODE_WORD:
                register_code(KC_LCTL);
                register_code(KC_BSPC);
                break;
        }

        refresh_mode_timer();
    } else {
        key_down = false;

        switch (current_mode) {
            case MODE_NONE:
                // It was a tap! Cancel the escalation timer and send Enter
                if (to_backspace_token) {
                    cancel_deferred_exec(to_backspace_token);
                    to_backspace_token = 0;
                }
                register_code(KC_ENT);
                unregister_code(KC_ENT);
                current_mode = MODE_ENTER;
                refresh_mode_timer();
                break;

            case MODE_ENTER:
                unregister_code(KC_ENT);
                break;

            case MODE_BACKSPACE:
                if (to_word_token) {
                    cancel_deferred_exec(to_word_token);
                    to_word_token = 0;
                }
                unregister_code(KC_BSPC);
                break;

            case MODE_WORD:
                unregister_code(KC_BSPC);
                unregister_code(KC_LCTL);
                break;
        }

        refresh_mode_timer();
    }

    return false;
}

void housekeeping_task_enter_bspc(void) {
    if (key_down) {
        // Key is still physically held -- keep the mode timer fully topped up
        // so nothing times out mid-hold.
        refresh_mode_timer();
        return;
    }

    if (current_mode != MODE_NONE &&
        timer_elapsed(mode_last_action_time) > ENTER_BSPC_TIMEOUT) {
        current_mode = MODE_NONE;
        refresh_mode_timer();
    }
}

#ifdef ENTER_BSPC_LED_INDEX
bool rgb_matrix_indicators_enter_bspc(void) {
    uint16_t elapsed  = timer_elapsed(mode_last_action_time);
    uint8_t  fraction = (elapsed >= ENTER_BSPC_TIMEOUT)
                            ? 0
                            : 255 - ((uint32_t)elapsed * 255 / ENTER_BSPC_TIMEOUT);

    if (fraction == 0) {
        return true;  // fully faded -- let the normal effect draw this LED
    }

    uint8_t r = 0, g = 0, b = 0;
    switch (current_mode) {
        case MODE_NONE:    // No LED - key is "unlit" when in no mode
            return true;
        case MODE_ENTER:   g = fraction; break;
        case MODE_BACKSPACE: b = fraction; break;
        case MODE_WORD:      r = fraction; break;
    }
    rgb_matrix_set_color(ENTER_BSPC_LED_INDEX, r, g, b);
    return true;
}
#endif