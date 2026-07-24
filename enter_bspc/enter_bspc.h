#pragma once

#include "quantum.h"

// Custom keycode is declared via qmk_module.json "keycodes" — QMK generates
// the ENT_BS enum value automatically, no manual enum needed here.

#ifndef ENTER_BSPC_TIMEOUT
#    define ENTER_BSPC_TIMEOUT 1000  // ms backspace-repeat stays armed after last use
#endif

#ifndef ENTER_BSPC_HOLD_TERM
#    define ENTER_BSPC_HOLD_TERM 150  // ms held before it's treated as "hold", not "tap"
#endif
