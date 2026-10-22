# Das Keyboard 4 (Professional)

An unofficial QMK port of the Metadot/DAS Keyboard 4 Professional, based on the
onboard NUC123 microcontroller.

* Keyboard Maintainer: [Giuseppe Iannello](https://github.com/giannello)
* Hardware Supported: DAS Keyboard 4 Professional (NUC123SD4AN0 onboard MCU)
* Hardware Availability: <https://www.daskeyboard.com/daskeyboard-4-professional>

Make example for this keyboard (after setting up your build environment):

    make metadot/das_keyboard_4:default

Flashing example for this keyboard:

    make metadot/das_keyboard_4:default:flash

## Layouts

Two keymaps ship with this keyboard:

* `default` — Windows layout.
* `macos` — Windows layout with the Win/Alt (and Right Alt/Win) keys swapped so
  the modifier order matches a Mac keyboard: `[Ctrl][Option][Cmd]`.

A `via` keymap is planned but deferred; see the "Deferred: VIA" section in
`NOTES.md` for what is required to enable it on the NUC123.

## Encoder

The volume knob uses the encoder map, defined in each keymap's `keymap.json`
(`config.features.encoder_map` enables it and `encoders` holds the rotation
keycodes). Rotate for volume up/down; press the knob to mute (the mute media key
is wired directly to a GPIO pin).

## Bootloader

Make sure you have the [nuc123-dfu-bootloader](https://github.com/giannello/nuc123-dfu-bootloader)
flashed to your board.

Enter the bootloader by holding `Esc`, `1`, and `A` together and plugging in
the keyboard.

The `D0` / "Suspend" media key sends `KC_SLEP` (no effect on macOS, per QMK's
`KC_SLEP` = HID System Control sleep).

## Notes

* The bootloader name is `numicro-dfu`. This is a *parked* name in the QMK
  schema and is expected to be renamed when the new Python-based bootloader is
  published (see the QMK TODO).
* See `NOTES.md` for the differences between the generic NUC123SD4AN0 board files
  used here and those from other NUC123-based keyboards.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools)
and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for
more information. Brand new to QMK? Start with the
[Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
