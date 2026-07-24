#include "enter_bspc.h"

static deferred_token hold_token       = 0;
static bool           held_as_backspace = false;
static bool           backspace_mode    = false;
static uint16_t       backspace_timer   = 0;

// Fires if the key is still held after ENTER_BSPC_HOLD_TERM ms — treat as hold.
static uint32_t hold_trigger_cb(uint32_t trigger_time, void *cb_arg) {
    register_code(KC_BSPC);
    held_as_backspace = true;
    backspace_mode    = true;
    backspace_timer   = timer_read();
    hold_token        = 0;
    return 0;  // one-shot, don't repeat
}

bool process_record_enter_bspc(uint16_t keycode, keyrecord_t *record) {
    if (keycode != ENT_BS) {
        return true;
    }

    // Disarm backspace-repeat mode if it's been idle too long.
    if (backspace_mode && timer_elapsed(backspace_timer) > ENTER_BSPC_TIMEOUT) {
        backspace_mode = false;
    }

    if (record->event.pressed) {
        if (backspace_mode) {
            // Already in backspace-repeat mode: skip the hold wait,
            // immediately delete on each tap.
            tap_code(KC_BSPC);
            backspace_timer = timer_read();
            return false;
        }

        // Start the hold-detection timer. If released before it fires,
        // this was a tap (handled on release below).
        held_as_backspace = false;
        hold_token = defer_exec(ENTER_BSPC_HOLD_TERM, hold_trigger_cb, NULL);
        return false;
    } else {
        if (hold_token) {
            // Released before the hold timer fired -> it was a tap.
            cancel_deferred_exec(hold_token);
            hold_token = 0;
            tap_code(KC_ENT);
        } else if (held_as_backspace) {
            // Was in hold mode -> release the held backspace.
            unregister_code(KC_BSPC);
            held_as_backspace = false;
            backspace_mode    = true;
            backspace_timer   = timer_read();
        }
        return false;
    }
}
