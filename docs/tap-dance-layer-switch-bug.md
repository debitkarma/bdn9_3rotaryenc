# Tap Dance Layer-Switch Bug (`TD_RESET` / layer 3)

This documents a real QMK bug we hit in the `db_testing` keymap, how we diagnosed it, and what we did about it. Relevant file: `keyboards/keebio/bdn9/keymaps/db_testing/keymap.c`.

## The custom function: `reset_to_zero` / `TD_RESET`

`TD_RESET` is a tap-dance key (`TD(TD_RESET)`) sitting on layer 3, row 1 / col 2, defined with a custom callback via `ACTION_TAP_DANCE_FN(reset_to_zero)` rather than the stock `ACTION_TAP_DANCE_LAYER_MOVE` macro, because its behavior has more tiers than "send a key on 1 tap, switch layers on 2 taps":

```c
void reset_to_zero(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 7 && IS_LAYER_ON(3)) {
        reset_keyboard();
    } else if (state->count >= 3) {
        layer_move(0);
        reset_tap_dance(state);
    } else if (state->count == 2) {
        reset_tap_dance(state);
        layer_move(0);
    } else if (state->count == 1) {
        tap_code(KC_0);
    }
}
```

### Intended behavior

| Taps | Behavior |
| -- | -- |
| 1 | Sends `KC_0` |
| 2 | Returns to layer 0 |
| 3–6 | Returns to layer 0 (and force-resets the dance's own state) |
| 7+ (while still on layer 3) | Full keyboard reset (`reset_keyboard()`) |

Same physical key (row 1 / col 2) on layer 0 is `TD_L3` (`ACTION_TAP_DANCE_LAYER_MOVE(KC_U, 3)`): 1 tap sends `KC_U`, 2 taps switches to layer 3. So this one physical key is `TD_L3` on layer 0 and `TD_RESET` on layer 3 - two different tap-dance actions sharing a matrix position, which turned out to matter a lot.

## What actually happened

**Symptom:** after tap-dancing back to layer 0 from layer 3 (2 taps on `TD_RESET`), the very next isolated tap of that same physical key did *nothing* - no `KC_U`, no visible reaction at all. A second isolated tap was needed before it reliably sent `KC_U` again. Every return trip through layer 3 cost one "wasted" tap on the way back.

**How we caught it:** temporarily instrumented the keymap with `uprintf()` calls (via `CONSOLE_ENABLE = yes`) logging every tap-dance callback and every raw keycode/layer/timestamp in `process_record_user`, then watched `qmk console` while reproducing the sequence:

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

### Root cause

QMK actually has a built-in mechanism specifically meant to prevent exactly this ([`action_layer.c`](https://github.com/qmk/qmk_firmware/blob/master/quantum/action_layer.c), `store_or_get_action()`, doc comment): "make sure the action triggered when the key is released is the same one as the one triggered on press... important when the layer is switched after the down event but before the up event." On a press, it caches the current layer for that matrix position; on a release, it reads back the cached value instead of the live layer state.

That protection gets defeated here because `process_tap_dance()` (core, `process_keycode/process_tap_dance.c`) **always returns `true`**, even after fully handling a `TD()` keycode. Since nothing else in the dispatch chain intercepts it either, every tap-dance press event also falls through to the standard action system's `store_or_get_action()` - a **second**, redundant cache write for the same key event. Sequence, for the 2nd tap of `TD_L3`:

1. `get_event_keycode()` runs first (layer still 0) → caches `layer=0` → correctly dispatches to `TD_L3`.
2. `process_tap_dance()` runs → `on_each_tap` fires `layer_move(3)` inline, **while this same press event is still being processed**.
3. `store_or_get_action()` then runs for this *same press event* (because of point above) → sees the *already-changed* layer (3) → **overwrites** the cache with `3`, clobbering the correct `0`.
4. When the key is released, the cache now reads back `3` → resolves to `TD_RESET` instead of `TD_L3`. Misrouted.

Because `TD_L3`'s own release never actually reaches it, its tap-dance state is left stuck (`pressed=true, count=2, finished=true`) and never zeroed. The next real press just increments the stale `count` to 3 instead of starting fresh at 1, and since `finished` is already (stale) `true`, the "on dance finished" callback never runs — the tap is silently swallowed, its only effect being to finally flush the corrupted state back to zero on release. The tap after *that* one is clean.

This isn't specific to `TD_L3`/`TD_RESET` - it applies to **any** tap-dance key whose layer-switch threshold can be reached while the key is still physically down, if a *different* tap-dance keycode sits at that same matrix position on the destination layer.

## The fix

Two separate call sites needed protecting, and everything else was deliberately left alone:

1. **`reset_to_zero`'s `count == 2` branch** (the return trip, layer 3 → 0): reordered to call `reset_tap_dance(state)` *before* `layer_move(0)`, confirmed on hardware to resolve the bug for this path.

2. **`TD_L3`** (the entry trip, layer 0 → 3 - the one actually caught in the log above): stock `ACTION_TAP_DANCE_LAYER_MOVE` calls `layer_move()` inline from `on_each_tap`, which is exactly the race described above. Since `process_tap_dance()`'s always-`true` return can't be changed from userspace (it's QMK core), the fix instead defers the layer switch to `on_reset` - invoked by this key's own *release*, a separate, later event. Releases only *read* the layer cache, they never write it, so nothing clobbers it afterward.

   ```c
   void safe_dual_role_each_tap(tap_dance_state_t *state, void *user_data) {
       if (state->count == 2) {
           state->finished = true; // don't move layers yet
       }
   }

   void safe_dual_role_reset(tap_dance_state_t *state, void *user_data) {
       tap_dance_dual_role_t *pair = (tap_dance_dual_role_t *)user_data;
       if (state->count == 1) {
           wait_ms(TAP_CODE_DELAY);
           unregister_code16(pair->kc);
       } else if (state->count == 2) {
           layer_move(pair->layer); // deferred until here
       }
   }

   #define ACTION_TAP_DANCE_LAYER_MOVE_SAFE(kc, layer)                                              \
       {                                                                                             \
           .fn = {safe_dual_role_each_tap, tap_dance_dual_role_finished, safe_dual_role_reset, NULL}, \
           .user_data = (void *)&((tap_dance_dual_role_t){kc, layer, layer_move}),                   \
       }
   ```

   Only `on_each_tap` needed replacing. `tap_dance_dual_role_finished` and the `tap_dance_dual_role_t` struct are QMK's own public API (declared in `process_tap_dance.h`), reused unmodified — no need to clone all three callbacks.

3. **`TD_L1`/`TD_L2`/`L1_RESET`/`L2_RESET`/`L3_RESET` were left on stock `ACTION_TAP_DANCE_LAYER_MOVE`, unchanged.** They don't pair with an `ACTION_TAP_DANCE_FN`-driven layer change the way `TD_L3`/`TD_RESET` do, and confirmed working as expected on hardware as-is - so only the one pairing that actually needed it (`TD_L3` ⟷ `TD_RESET`, both touching layer 3) got the custom treatment.

## Takeaway for future `ACTION_TAP_DANCE_FN` callbacks that change layers

If a custom tap-dance callback calls `layer_move()` (or similar) based on `state->count`, and the same physical key maps to a *different* tap-dance action on the layer being switched to/from, don't call the layer switch immediately from a spot that can run while the key is still physically held:

- If the layer switch lives in `on_dance_finished` (like `reset_to_zero`, no `on_each_tap` defined) - which normally only fires after the key is released anyway, since it's driven by the tapping-term timeout - call `reset_tap_dance(state)` right before the layer switch as cheap insurance against the rarer case where it's triggered by *another* key interrupting the dance while this one is still held.
- If the layer switch needs to happen inline from `on_each_tap` for a snappy/instant double-tap feel (like the stock dual-role macro), don't call `layer_move()` there at all - only mark `state->finished = true`, and do the actual `layer_move()` in `on_reset` instead, so it happens after this key's own release.
