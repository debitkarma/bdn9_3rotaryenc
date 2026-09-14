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

To view console/debug output (keymap needs `CONSOLE_ENABLE = yes` in `rules.mk`, see [Debugging](#debugging) below):
```
qmk console
```
OR
```
hid_listen
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

#### Debugging (stale - see [Tap Dance Layer-Switch Bug](#tap-dance-layer-switch-bug-current-state) below)

The `db_testing` keymap has `CONSOLE_ENABLE = yes` set in its `rules.mk`, and `keymap.c` has been instrumented to log to the QMK console (`qmk console` / `hid_listen`) via `uprintf()`:

* `reset_to_zero` (the `TD_RESET` tap dance function) logs `count`, `pressed`, `interrupted`, and a timestamp every time it fires.
* `TD_L1`, `TD_L2`, `TD_L3`, `L1_RESET`, `L2_RESET`, `L3_RESET` use a custom `ACTION_TAP_DANCE_LAYER_MOVE_DEBUG` macro (originally a straight clone of QMK's built-in `ACTION_TAP_DANCE_LAYER_MOVE` dual-role logic from `process_tap_dance.c`, now patched - see below), tagged with a name string, logging on every `each_tap` / `finished` / `reset` callback.
* `process_record_user` logs every raw keycode/row/col/pressed/layer/timestamp before any other handling, to correlate against the tap-dance-specific logs above.

This instrumentation is temporary/diagnostic - remove it (or drop `CONSOLE_ENABLE`) once the tap dance bug below is confirmed fixed on hardware.

#### Bug: tap dance layer-switch key needs a "throwaway" tap (stale - see [Tap Dance Layer-Switch Bug](#tap-dance-layer-switch-bug-current-state) below)

**Symptom:** layer3key (row1/col2 - `TD_RESET` on layer 3, `TD_L3` on layer 0) tap-danced back to layer 0 correctly (2 taps on `TD_RESET`), but the very next isolated tap of that same physical key did nothing at all. A second isolated tap was needed before it would reliably send `KC_U` again.

**Console output that caught it** (`qmk console`, timestamps in ms, keycodes are `TD()` indices - `0x5702` = `TD_L3`, `0x5706` = `TD_RESET`):

```
t=140301  press   kc=0x5702 layer=0   → [TD_L3] each_tap count=1
t=140360  release kc=0x5702 layer=0
t=140444  press   kc=0x5702 layer=0   → [TD_L3] each_tap count=2      ← layer_move(3) fired here, inline, key still down
t=140476  release kc=0x5706 layer=3   ← same physical key, but layer already flipped - release resolves to TD_RESET instead!
  ... (two clean TD_RESET taps on layer 3) ...
t=144955  [TD_RESET] finished count=2 pressed=0 interrupted=0        ← timeout-driven layer_move(0), works correctly
t=146572  press   kc=0x5702 layer=0   → [TD_L3] each_tap count=3      ← count picked up from the stale "2", not a fresh 1
t=146655  release kc=0x5702 layer=0   → [TD_L3] reset count=3        ← no "finished" line at all - nothing was ever sent
t=148507  press   kc=0x5702 layer=0   → [TD_L3] each_tap count=1      ← finally a clean cycle
t=149008           (500ms later)      → [TD_L3] finished count=1 → registers KC_U
                                       → [TD_L3] reset count=1    → unregisters KC_U
```

**Root cause:** QMK's `ACTION_TAP_DANCE_LAYER_MOVE` dual-role logic calls `layer_move()` synchronously inside `on_each_tap`, the instant `count` hits 2 - i.e. while the 2nd tap's key is still physically held down. QMK looks up the keycode for a release event fresh, against whatever layer is active *at that moment*; it does not remember what layer was active when the key went down. So the release half of that same physical tap gets looked up against the *new* layer and resolves to whatever other key sits at that same matrix position on the destination layer (here, `TD_RESET` on layer 3) - a different tap-dance action entirely.

Because of that, `TD_L3`'s own action state never receives its matching release event, so it's left stuck (`pressed=true, count=2, finished=true`) and never gets zeroed. The next real press of that key just increments the stale `count` to 3, and since `finished` is already (stale) `true`, `on_dance_finished` never even runs - the tap is silently swallowed, its only effect being to finally flush the corrupted state back to zero on release. The tap after *that* one is clean and behaves correctly. Hence: one "wasted" tap, every time.

This isn't specific to `TD_L3`/`TD_RESET` - any dual-role tap dance whose layer-switch threshold can be reached while the key is still down will self-corrupt if a different tap-dance keycode occupies that same physical position on the destination layer (so `TD_L1`/`L1_RESET` and `TD_L2`/`L2_RESET` were presumably equally affected, just not manually verified the same way).

**Fix applied:** in the custom `debug_dual_role_*` functions (userspace clones in `keymap.c` - not QMK core, so this is fully editable), moved the actual `layer_move()` call out of `on_each_tap` and into `on_reset`, keeping only `state->finished = true` in `on_each_tap`. This defers the layer switch until *after* this key's own release has already been dispatched (correctly, against the still-old layer), instead of racing it. Pending hardware re-test to confirm the fix.

#### Tap Dance Layer-Switch Bug (current state)

The two sections above are stale - they describe an earlier, disproven theory (release lookups aren't cached - they are) and an intermediate fix that turned out to only address part of the problem. The confirmed root cause, the full diagnostic log, the actual fix, and a general takeaway for future `ACTION_TAP_DANCE_FN` callbacks are written up in [`docs/tap-dance-layer-switch-bug.md`](docs/tap-dance-layer-switch-bug.md).

Short version of where things landed, confirmed working on hardware:

* `reset_to_zero`'s `count == 2` branch (`TD_RESET`, layer 3 → 0) calls `reset_tap_dance(state)` before `layer_move(0)`.
* `TD_L3` (layer 0 → 3 - the pairing that actually races with `TD_RESET`) uses a custom `ACTION_TAP_DANCE_LAYER_MOVE_SAFE` macro that defers its `layer_move()` to `on_reset` instead of firing it inline from `on_each_tap`, reusing QMK's own `tap_dance_dual_role_finished`/`tap_dance_dual_role_t` unmodified.
* `TD_L1`/`TD_L2`/`L1_RESET`/`L2_RESET`/`L3_RESET` are back on stock `ACTION_TAP_DANCE_LAYER_MOVE`, unchanged - they don't pair with any `ACTION_TAP_DANCE_FN`-driven layer change, so the bug doesn't apply to them, confirmed working as-is.
* The diagnostic `uprintf`/`CONSOLE_ENABLE` instrumentation described above has been removed from `keymap.c`.

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
* [Debugging/console output](https://docs.qmk.fm/faq_debug)
* ["Extra" keycodes for macro use](https://www.reddit.com/r/olkb/comments/6yxgcd/qmkcreating_a_keymap_for_use_as_macro_keyboard/)
* [Full list of keycodes available in QMK](https://docs.qmk.fm/keycodes)
* [Official Keebio BDN9 Rev2 build instructions](https://github.com/qmk/qmk_firmware/tree/master/keyboards/keebio/bdn9)
* [Instructions to use Make with qmk building](https://docs.qmk.fm/getting_started_make_guide)
* [Leader key instructions](https://docs.qmk.fm/features/leader_key)
* [Feature layers docs](https://docs.qmk.fm/feature_layers)
