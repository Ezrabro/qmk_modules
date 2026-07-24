# Enter / Hold-to-Backspace (v5 — tap to enter mode, hold to repeat)

Put `ENT_BS` in your keymap where you want Enter to normally be.

## Modes

The key is always in one of four states:

- **No mode (unlit)** — Initial state. Tap sends Enter and switches to Enter mode. Hold escalates to Backspace then Word mode.
- **Enter mode (green)** — Tap or hold both send Enter. **Holding repeats Enter**.
- **Backspace mode (blue)** — Tap or hold both send Backspace. **Holding repeats Backspace**.
- **Word mode (red)** — Tap or hold both send Ctrl+Backspace. **Holding repeats Ctrl+Backspace**.

## Operation

1. **From No mode:**
   - **Tap** → sends Enter, switches to Enter mode
   - **Hold** → waits `ENTER_BSPC_HOLD_TERM` ms, then sends Backspace and switches to Backspace mode
   - **Hold further** → waits additional `ENTER_BSPC_WORD_HOLD_TERM` ms, then sends Ctrl+Backspace and switches to Word mode

2. **From Enter/Backspace/Word mode:**
   - **Tap** → sends that mode's action (Enter, Backspace, or Ctrl+Backspace)
   - **Hold** → **repeats** that action (like a normal key)
   - No escalation happens from these modes

3. **Timeout:** After `ENTER_BSPC_TIMEOUT` ms of inactivity, the mode reverts to No mode.

## LED feedback

Set `ENTER_BSPC_LED_INDEX` to this key's LED index to enable the fading indicator. The LED fades from full color to off over `ENTER_BSPC_TIMEOUT` ms, showing how much time remains before reverting to No mode.

## Requirements

- `DEFERRED_EXEC_ENABLE = yes` (enabled automatically by the module)
- `RGB_MATRIX_ENABLE = yes` only if using LED feedback