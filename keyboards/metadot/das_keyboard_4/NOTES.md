# Board / platform notes

This keyboard targets the onboard **NUC123SD4AN0** (Nuvoton NUC123, Cortex-M0).

## Why board files are shipped in this directory

`platforms/chibios/platform.mk` resolves `BOARD` by searching
`boards/$(BOARD)/board.mk` under the keyboard's own path(s). `NUC123SD4AN0` is
**not** shipped inside `platforms/chibios/boards/`, so each NUC123 keyboard must
carry its own copy. `MCU = NUC123` (via `processor` in `keyboard.json`) makes
`platforms/chibios/mcu_selection.mk` default `BOARD ?= NUC123SD4AN0`.

The files in `boards/NUC123SD4AN0/` are copied **verbatim** from
`keyboards/ducky/one2mini/1861st/boards/NUC123SD4AN0/` (a known-good NUC123
keyboard in QMK).

## Ducky `one2mini/1861st` vs the old `das_keyboard` port

Compared the generic NUC123SD4AN0 board files (ducky vs the old
`keyboards/das_keyboard/boards/NUC123SD4AN0/`, which the old
`4_professional/` keyboard reached via its parent directory):

| File | Ducky `one2mini/1861st` vs old `das_keyboard` | Verdict |
|------|-----------------------------------------------|---------|
| `boards/NUC123SD4AN0/board.c`  | identical | same |
| `boards/NUC123SD4AN0/board.mk` | identical | same |
| `boards/NUC123SD4AN0/board.h`  | differs in **2 comment lines only** (PD4/PD5 annotated as `input pullup` in the old copy vs `output` in ducky) | cosmetic — kept ducky's verbatim |

Compared the ChibiOS config headers (ducky `one2mini/1861st` vs old
`das_keyboard/4_professional`):

| File | Difference | This keyboard |
|------|-----------|---------------|
| `mcuconf.h` | **identical** | copied ducky's |
| `chconf.h`  | old port shipped one (defined `CH_CFG_ST_TIMEDELTA 0`, commented out `CH_CFG_ST_FREQUENCY` / `CH_CFG_USE_WAITEXIT`); ducky enables `ST_FREQUENCY 10000` + `USE_WAITEXIT TRUE` | **copied old port's verbatim** — required, the QMK default (`ST_TIMEDELTA 2`) produced a non-working build |
| `config.h`  | **only** license/comment text differs; the real line `GPIO_INPUT_PIN_DELAY (NUC123_HCLK / 6 / 1000000L)` is identical in both | rewritten from scratch with just the one needed define |

Effective `chconf.h` values: `CH_CFG_ST_FREQUENCY 100000`, `CH_CFG_ST_TIMEDELTA 0`,
`CH_CFG_USE_WAITEXIT FALSE`.

## GPIO input sampling

NUC123 GPIO input sampling is slower than the platform default, so `config.h`
sets `GPIO_INPUT_PIN_DELAY (NUC123_HCLK / 6 / 1000000L)` — identical to both the
ducky and the old das_keyboard ports.

## Bootloader

`bootloader: numicro-dfu` (`platforms/chibios/bootloaders/numicro_dfu.c`).
`numicro-dfu` is a *parked* name in the QMK schema and is expected to be renamed
when the new Python-based bootloader is published (QMK TODO).

## Deferred: VIA

A `via` keymap is planned but not yet enabled. Getting VIA to work on the
NUC123 requires two hardware-level changes, both of which need validation on a
real board:

1. **Persistent EEPROM via flash wear-leveling.** The NUC123 has no EEPROM, and
   QMK's `EEPROM_DRIVER = vendor` falls back to *transient* (RAM-only) EEPROM,
   which is too small for dynamic keymaps. The fix is to use flash-backed
   wear-leveling:

   ```
   # in keymap rules.mk
   EEPROM_DRIVER = wear_leveling
   WEAR_LEVELING_DRIVER = embedded_flash

   # in keymap config.h
   #define WEAR_LEVELING_EFL_FLASH_SIZE 0x10000   # 64KB flash
   #define BACKING_STORE_WRITE_SIZE 4             # NUC123 flash line size
   #define WEAR_LEVELING_BACKING_SIZE 4096        # 4KB backing -> 2KB logical EEPROM
   ```

   Notes:
   * QMK's `wear_leveling_efl_config.h` checks `QMK_MCU_FAMILY_NUC123`, but the
     family macro is actually `QMK_MCU_FAMILY_NUMICRO`, so
     `BACKING_STORE_WRITE_SIZE` must be set explicitly.
   * The wear-leveling backing store is placed at the *top* of the 64KB APROM.
     The firmware is ~22KB, so there is ample room and no overlap with the
     `numicro-dfu` bootloader (which lives in LDROM).

2. **A 3rd USB endpoint for VIA's Raw HID.** NUC123(AN) parts default to only
   2 user USB endpoints (`USB_MAX_ENDPOINTS 2`). VIA needs Raw HID (IN+OUT)
   *plus* a shared keyboard endpoint — 3 endpoints. Enabling the Nuvoton
   endpoint workaround exposes a 3rd:

   ```
   # in mcuconf.h
   #define NUC123_USB_WORKAROUND 1
   ```

   ⚠ This remaps the USB endpoint addressing for **all** keymaps, including
   `default`/`macos`, so those would need re-validation on hardware.

Both items were verified to produce a successful `via` build once applied. They
are intentionally left out for now so the working `default`/`macos` builds stay
on the proven, non-workaround configuration.
