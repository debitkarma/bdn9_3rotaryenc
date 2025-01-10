# QMK Config - BDN9 Rev2 w/ 3 Rotary Encs

This is a repo for the configuration of the firmware for a [keeb.io BDN9 Revision 2 Macropad](https://docs.keeb.io/bdn9-rev2-build-guide), which supports up 3 rotary encoders in the top 3 slots (the top row). I have all 3 installed, so this is to build fw for my specific situation.

### Physical Layout

| rotary encoder 0 | rotary encoder 1 | rotary encoder 2 |
|------------------|------------------|------------------|
| kailh silver     | kailh red        | kailh black      |
| kailh blue       | kailh green      | kailh white      |


### Functions

Order for rotary encoders is as follows:

`clockwise action / counter-clockwise action`

#### Layer 0 (default)

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | vol up / down | arrow down / up | pg down / up |
| color normally | none | none | none |
| click   | mute | home | end |
| click color | red while muted | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | F13 | F14 | F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | layer 1 | layer 2 | layer 3 |
| color | whole layer red | whole layer green | whole layer blue |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | F16 | F17 | F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | layer 4 | layer 5 | layer 6 |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

#### Layer 1 (meta functions)

\* = F22 used as modifer key in this layer. F22 down will be sent, then the key mentioned, then F22 up/release will be sent.

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | \*F19 / \*F20 | \*1 / \*2 | \*4 / \*5 |
| color normally | none | none | none |
| click   | \*F21 | \*3 | \*6 |
| click color | red | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | \*F13 | \*F14 | \*F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | TO layer 0 | -- | -- |
| color | off | whole layer green | whole layer blue |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | \*F16 | \*F17 | \*F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | -- | -- | -- |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

#### Layer 2 (meta functions)

@ = F23 used as modifer key in this layer. F23 down will be sent, then the key mentioned, then F23 up/release will be sent.

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | @F19 / @F20 | @1 / @2 | @4 / @5 |
| color normally | none | none | none |
| click   | @F21 | @3 | @6 |
| click color | red | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | @F13 | @F14 | @F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | -- | TO layer 0 | -- |
| color | whole layer red | off | whole layer blue |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | @F16 | @F17 | @F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | -- | -- | -- |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

#### Layer 3 (meta functions)

& = F24 used as modifer key in this layer. F24 down will be sent, then the key mentioned, then F24 up/release will be sent.

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | &F19 / &F20 | &1 / &2 | &4 / &5 |
| color normally | none | none | none |
| click   | &F21 | &3 | &6 |
| click color | red | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | &F13 | &F14 | &F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | -- | -- | TO layer 0 |
| color | whole layer red | whole layer green | off |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | &F16 | &F17 | &F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | -- | -- | -- |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

## TO DO

* Add tap dancing ability
* Tap dancing routes to other layers
* Cancel layer back to 0
* Create function (if possible) to wrap keycodes in another F key
* Add per key lighting for layer 0
* Add lighting toggle for Volume/Mute
* Add per-layer lighting for other layers
* Create AutoHotKey script for handling of all the keys
* Look into 2-way comm to board to sync status of mute to the LED
* Consider adding layers 4-6

## Thoughts

#### QMK Commands

To build:

```
cd ~/qmk_firmware
make keebio/bdn9/rev2:debitkarma
```
OR
```
qmk compile -kb keebio/bdn9/rev2 -km debitkarma
```

To flash:
```
qmk flash -kb keebio/bdn9/rev2 -km debitkarma
```

* [BDN9 GH issue with example output](https://github.com/qmk/qmk_firmware/issues/11265)
* [Frosty Flake compile/flash instructions, for comparison](https://github.com/qmk/qmk_firmware/blob/master/keyboards/bpiphany/frosty_flake/readme.md)
* [`make` instructions from official docs](https://docs.qmk.fm/getting_started_make_guide)

#### Wrapping
Use `process_record_user()` and `post_process_record_user()` functions to wrap inputs in F22, F23, or F24 keys.

1. `process_record_user()` checks for input and which layer is active
1. Based on which layer is active, sets a flag
1. Wraps the keycode and sends it
1. `post_process_record_user()` unsets the wrapping key
1. Returns to layer 0? Perhaps we want multiple inputs so we don't reset the layer back to 0?

TRY:
* `unregister_code` for the release of the wrapping key, without using post process
* try using `tap_keycode` for the actual tapped key
* try abstracting the wrapping to a generic function that takes a keycode
* check `switch / case` statements and add the rotary encoder keycodes (for non-0 layers)
* use custom keycodes for the non-0 layer rotary encoders for easier handling
* try using rotary encoder to cycle through the layers?

#### Lighting
* Split lighting logic into another file and `#include`?
* Handle logic for checking which layer is active and what color the lights are
* qmk functions should be using `rgb_matrix_*` functions (NOT `rgb_light_*` functions, even for the backlight)
* LEDS are in a single matrix, addressed as 0-8 for the face keys, and 9, 10 for the underlighting

#### Input Handling in AHK
* AHK should handle F22/F23/F24 wrapped keys.
* AHK should basically pass custom shortcuts to specific applications:
  * OBS (scene changes, mic adjustments, alert handling)
  * Windows (volume)
  * Discord (volume)
  * Emulation ?
* AHK should handle `F22 down, F22 up, _____, F23 down, F23 up` as a sequence (used on the planck ez glow temporarily

# Resource Links

* [Macro guide explaining with custom code: part 1, macros](https://getreuer.info/posts/keyboards/macros/index.html)
* [Macro guide explaining with custom code: part 2, triggers](https://getreuer.info/posts/keyboards/triggers/index.html)
* [Macro guide explaining with custom code: part 3, advanced effects](https://getreuer.info/posts/keyboards/macros3/index.html)
* [Lighting Layers docs](https://docs.qmk.fm/features/rgblight#lighting-layers)
* [Macro instructions, with complex examples](https://docs.qmk.fm/feature_macros)
* [Rotary Encoder definitions](https://docs.qmk.fm/features/encoders)
* [Tap Dance functionality](https://docs.qmk.fm/features/tap_dance)
* ["Extra" keycodes for macro use](https://www.reddit.com/r/olkb/comments/6yxgcd/qmkcreating_a_keymap_for_use_as_macro_keyboard/)
* [Full list of keycodes available in QMK](https://docs.qmk.fm/keycodes)
* [Official Keebio BDN9 Rev2 build instructions](https://github.com/qmk/qmk_firmware/tree/master/keyboards/keebio/bdn9)
* [Instructions to use Make with qmk building](https://docs.qmk.fm/getting_started_make_guide)
* [Leader key instructions](https://docs.qmk.fm/features/leader_key)
* [Feature layers docs](https://docs.qmk.fm/feature_layers)
