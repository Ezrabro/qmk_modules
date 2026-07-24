# OpenRGB QMK Protocol Community Module (Protocol Version D)

This is a QMK Community Module to provide the OpenRGB QMK protocol (version D).

Add the following to the list of modules in your `keymap.json` to enable this module:

```json
{
    "modules": ["openrgb"]
}
```

## Caveat #1 - USB Endpoint Sizing

This version of the OpenRGB protocol requires that the raw endpoint to be 64 bytes (rather than the standard 32), and this isn't noramlly configurable.

The module automatically patches `tmk_core/protocol/usb_descriptor.h` at build time — replacing the bare `#define RAW_EPSIZE 32` with a guarded version so the endpoint size can be overridden:

```c
#ifndef RAW_EPSIZE
#    define RAW_EPSIZE 32
#endif
```

No manual file edits are required. 

## Caveat #2 - RGB Matrix Animations

OpenRGB hard codes the animations that are supported, so some of the newer animations are not available.
