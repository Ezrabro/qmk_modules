#pragma once

#include "quantum.h"

// Custom keycode is declared via qmk_module.json "keycodes" — QMK generates
// the ENT_BS enum value automatically, no manual enum needed here.

#ifndef ENTER_BSPC_HOLD_TERM
#    define ENTER_BSPC_HOLD_TERM 150   // ms held (from Enter mode) before -> Backspace mode
#endif

#ifndef ENTER_BSPC_WORD_HOLD_TERM
#    define ENTER_BSPC_WORD_HOLD_TERM 500  // ms further held (same press) before -> Word mode
#endif

#ifndef ENTER_BSPC_TIMEOUT
#    define ENTER_BSPC_TIMEOUT 1500   // ms of inactivity before mode fades out / reverts
#endif

#ifndef ENTER_BSPC_LED_INDEX
// Set this to the LED index of the ENT_BS key (see your keyboard's
// info.json g_led_config) to enable the fading mode indicator.
// Leave undefined to disable LED feedback entirely.
// #define ENTER_BSPC_LED_INDEX 42
#endif