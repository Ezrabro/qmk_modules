# Enter / Hold-to-Backspace (v2 — no LT() hack)

Put `ENT_BS` in your keymap where you want Enter to normally be.

- **Tap** → sends Enter.
- **Hold** (past `ENTER_BSPC_HOLD_TERM`, default 150ms) → holds Backspace
  (OS handles the repeat-while-held behavior).
- After releasing from a hold, a short window (`ENTER_BSPC_TIMEOUT`, default
  1000ms) stays armed so a quick re-tap keeps deleting one character at a
  time, without needing to hold again.

Unlike the original version, this does **not** repurpose `LT(0, KC_ENT)`.
It's a genuine custom keycode with its own private hold-timer, so it
doesn't touch `TAPPING_TERM`, doesn't interact with QMK's layer-tap
tap-count machinery, and doesn't require wiring anything into a shared
`get_tapping_term()`.

## Use

```json
{
    "modules": ["azar/enter_bspc"]
}
```

Then place `ENT_BS` in your keymap wherever `KC_ENT` was.

## Configuration

| Define                  | Default | Description                                      |
|--------------------------|---------|---------------------------------------------------|
| `ENTER_BSPC_HOLD_TERM`   | `150`   | ms held before it counts as "hold" instead of tap |
| `ENTER_BSPC_TIMEOUT`     | `1000`  | ms before backspace-repeat mode disarms           |

## Requirements

- `DEFERRED_EXEC_ENABLE = yes` (enabled automatically by the module)
