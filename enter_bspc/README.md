# Enter / Hold-to-Backspace (v4 — persistent modes + fading indicator)

Put `ENT_BS` in your keymap where you want Enter to normally be.

## Modes

The key is always in one of three persistent modes. The mode sticks
across multiple presses until it times out from inactivity.

- **Enter mode (green)** — tap or hold both send Enter.
- **Backspace mode (blue)** — tap or hold both send one Backspace.
- **Word mode (red)** — tap or hold both send one Ctrl+Backspace
  (deletes a whole word).

## Escalating modes

Escalation only happens during **one continuous hold**, starting from
Enter mode:

1. Press and hold past `ENTER_BSPC_HOLD_TERM` (without releasing) →
   sends one Backspace immediately (so you feel the mode change),
   switches to Backspace mode.
2. Keep holding the *same* press past a further
   `ENTER_BSPC_WORD_HOLD_TERM` → sends one Ctrl+Backspace, switches to
   Word mode.
3. Release whenever — nothing extra happens on release; the action
   already fired during the hold.

Once you're already in Backspace or Word mode, a **new** press (tap or
hold, doesn't matter) just repeats that mode's action immediately —
there's no re-escalation on a fresh press. To go from Backspace mode to
Word mode again, you'd need a fresh long hold from Enter mode, or just
stay in whichever mode suits what you're doing.

## Timeout and fading indicator

Every press in the current mode resets its LED brightness to full and
restarts the countdown. The LED continuously fades from full color
toward off over `ENTER_BSPC_TIMEOUT` ms — a visual readout of how much
longer the current mode will stay active before reverting to Enter mode.

## LED feedback

Set `ENTER_BSPC_LED_INDEX` to this key's LED index (find it in your
keyboard's `info.json` under `g_led_config`) to enable the fading
indicator. Leave it undefined to disable LED feedback entirely (the
module still works, just silently). Requires `RGB_MATRIX_ENABLE`.

## Use

```json
{
    "modules": ["azar/enter_bspc"]
}
```

Then place `ENT_BS` in your keymap wherever `KC_ENT` was, and set
`ENTER_BSPC_LED_INDEX` in your keymap's `config.h`.

## Configuration

| Define                     | Default   | Description                                                        |
|------------------------------|-----------|-------------------------------------------------------------------------|
| `ENTER_BSPC_HOLD_TERM`       | `150`     | ms held (from Enter mode) before escalating to Backspace mode           |
| `ENTER_BSPC_WORD_HOLD_TERM`  | `500`     | ms further held (same press) before escalating to Word mode              |
| `ENTER_BSPC_TIMEOUT`         | `1500`    | ms of inactivity before the mode fades out / reverts to Enter            |
| `ENTER_BSPC_LED_INDEX`       | *(unset)* | LED index to show the fading mode indicator; unset = no LED feedback   |

## Requirements

- `DEFERRED_EXEC_ENABLE = yes` (enabled automatically by the module)
- `RGB_MATRIX_ENABLE = yes` only if using LED feedback