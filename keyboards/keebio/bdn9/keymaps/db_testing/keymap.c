/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 /* This keymap is a test that uses simple key assignments
  * to test the functionality of our Tap Dance actions,
  * layers, RGB lighting, and the keystroke wrapping.
  * This is not for actual use alongside a keyboard!!!
 */
#include QMK_KEYBOARD_H
static uint8_t f22_tracker;
static uint8_t f23_tracker;
static uint8_t f24_tracker;

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum {
  TD_L1,
  TD_L2,
  TD_L3,
  L1_RESET,
  L2_RESET,
  L3_RESET,
  TD_RESET
};

// "ENCODER_MAP_ENABLE = yes" -> rules.mk
// This allows for per-layer encoder definitions
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_PGUP, KC_PGDN),
        ENCODER_CCW_CW(MS_WHLU, MS_WHLD),
    },
    [1] = {
        ENCODER_CCW_CW(KC_MINUS, KC_EQUAL),
        ENCODER_CCW_CW(KC_MINUS, KC_EQUAL),
        ENCODER_CCW_CW(KC_MINUS, KC_EQUAL),
    },
    [2] = {
        ENCODER_CCW_CW(KC_LBRC, KC_RBRC),
        ENCODER_CCW_CW(KC_LBRC, KC_RBRC),
        ENCODER_CCW_CW(KC_LBRC, KC_RBRC),
    },
    [3] = {
        ENCODER_CCW_CW(KC_QUOT, KC_COMM),
        ENCODER_CCW_CW(KC_QUOT, KC_COMM),
        ENCODER_CCW_CW(KC_QUOT, KC_COMM),
    },
};
#endif

// Reset to zero whenever tapping a lot in a row
void reset_to_zero(tap_dance_state_t *state, void *user_data) {
    // switch(get_highest_layer(layer_state|default_layer_state)) {
    //     case 3:
    //     case 2:
    //     case 1:
    //     default:
    // }
    if (state->count >= 7 && IS_LAYER_ON(3)) {
        // Set board to reset/flash if more than 7 taps happen on a tapdance key
        reset_keyboard();
    } else if (state->count >= 3) {
        // Reset the keyboard to Layer 0 if more than 3 taps happen on a tapdance key
        reset_tap_dance(state);
        layer_move(0);
    } else if (state->count == 2) {
        reset_tap_dance(state);
        layer_move(0);
    } else if (state->count == 1) {
        tap_code(KC_0);
    }
}

// TD_L1/TD_L2/TD_L3/L1_RESET/L2_RESET/L3_RESET keep the stock
// ACTION_TAP_DANCE_LAYER_MOVE behavior (send kc on 1 tap, layer_move on 2
// taps) - but stock's on_each_tap calls layer_move() inline, while the 2nd
// tap's key is still being processed as part of *this* press event. Since
// process_tap_dance() always returns true, this same press event always
// falls through afterward to the standard action system's
// store_or_get_action(), which re-caches whatever layer is active *at that
// point* for this matrix position. If we've already switched layers by
// then - no matter how, or in what order, inside on_each_tap - that call
// clobbers the correct press-time cache entry. The next release then gets
// looked up against the wrong layer and can resolve to an unrelated key
// sharing this matrix position (see README "Bug: tap dance layer-switch key
// needs a throwaway tap").
//
// So the layer switch itself cannot happen inside on_each_tap at all - it
// has to wait until *this key's own release*, which is a separate, later
// event: releases only *read* the layer cache, never write it, so nothing
// clobbers it afterward. on_reset (called right after that release, once
// the dance is finished) is the right place. That means on_each_tap only
// marks the dance finished (no layer_move here), and on_reset is where the
// deferred layer_move() actually happens - tap_dance_dual_role_finished
// (QMK's own, process_tap_dance.h) is still reused unmodified for the
// count==1 case.
void safe_dual_role_each_tap(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        state->finished = true;
    }
}

void safe_dual_role_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_dual_role_t *pair = (tap_dance_dual_role_t *)user_data;
    if (state->count == 1) {
        wait_ms(TAP_CODE_DELAY);
        unregister_code16(pair->kc);
    } else if (state->count == 2) {
        layer_move(pair->layer);
    }
}

#define ACTION_TAP_DANCE_LAYER_MOVE_SAFE(kc, layer)                                              \
    {                                                                                             \
        .fn = {safe_dual_role_each_tap, tap_dance_dual_role_finished, safe_dual_role_reset, NULL}, \
        .user_data = (void *)&((tap_dance_dual_role_t){kc, layer, layer_move}),                   \
    }

// Tap Dance functions
// defining tap dance actions prior to keymap
// Only TD_L3 (pairs with TD_RESET/reset_to_zero on layer 3) uses the safe,
// custom each_tap/reset fix above. TD_L1/TD_L2/L1_RESET/L2_RESET/L3_RESET
// stay on stock ACTION_TAP_DANCE_LAYER_MOVE - they don't pair with any
// ACTION_TAP_DANCE_FN-driven layer change, so the bug doesn't apply to them.
tap_dance_action_t tap_dance_actions[] = {
    [TD_L1] = ACTION_TAP_DANCE_LAYER_MOVE(KC_S, 1),
    [TD_L2] = ACTION_TAP_DANCE_LAYER_MOVE(KC_T, 2),
    [TD_L3] = ACTION_TAP_DANCE_LAYER_MOVE_SAFE(KC_U, 3),
    [L1_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_A, 0),
    [L2_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_H, 0),
    [L3_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_O, 0),
    [TD_RESET] = ACTION_TAP_DANCE_FN(reset_to_zero)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_MUTE  , KC_HOME  , KC_END,
        TD(TD_L1), TD(TD_L2), TD(TD_L3),
        KC_6     , KC_W     , KC_X
    ),
    [1] = LAYOUT(
        KC_SPC       , KC_SPC   , KC_SPC,
        TD(L1_RESET) , KC_B     , KC_C,
        KC_D         , KC_E     , KC_F
    ),
    [2] = LAYOUT(
        KC_BSLS , KC_BSLS         , KC_BSLS  ,
        KC_G    , TD(L2_RESET)    , KC_I,
        KC_J    , KC_K            , KC_L
    ),
    [3] = LAYOUT(
        KC_SCLN , KC_SCLN   , QK_BOOT  ,
        KC_M    , KC_N      , TD(TD_RESET),
        KC_P    , KC_Q      , KC_R
    ),
};

// F-key wrapping functionality
// On key down
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_A ... KC_F1:
        // grab layer from the key event
        uint8_t layer = read_source_layers_cache(record->event.key);
        switch (layer) {
        case 3:
            if (record->event.pressed) {
            register_code(KC_F24);
            f24_tracker++;
            tap_code(keycode);
            return false;
            }
            break;
        case 2:
            if (record->event.pressed) {
            register_code(KC_F23);
            f23_tracker++;
            tap_code(keycode);
            return false;
            }
            break;
        case 1:
            if (record->event.pressed) {
            register_code(KC_9);
            f22_tracker++;
            tap_code(keycode);
            return false;
            }
            break;
        }
    }
    return true;
};
// For key up
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
    case KC_A ... KC_F1:
        uint8_t layer = read_source_layers_cache(record->event.key);
        switch (layer) {
        case 3:
            if (!record->event.pressed) {
                f24_tracker--;
                if (!f24_tracker) {
                    unregister_code(KC_F24);
                }
            }
            break;
        case 2:
            if (!record->event.pressed) {
                f23_tracker--;
                if (!f23_tracker) {
                    unregister_code(KC_F23);
                }
            }
            break;
        case 1:
            if (!record->event.pressed) {
                f22_tracker--;
                if (!f22_tracker) {
                    unregister_code(KC_9);
                }
            }
            break;
        }
    }
}

// Used this for testing the individual LEDs in the matrix
/*
bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
    rgb_matrix_set_color_all(255,0,0);
    rgb_matrix_set_color(9, 0, 0, 255);
    rgb_matrix_set_color(10, 0, 255, 0);
    return true;
}
*/

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch(get_highest_layer(layer_state|default_layer_state)) {
        case 3:
            RGB_MATRIX_INDICATOR_SET_COLOR(9, 0, 0, 255);
            RGB_MATRIX_INDICATOR_SET_COLOR(10, 0, 0, 255);
            break;
        case 2:
            RGB_MATRIX_INDICATOR_SET_COLOR(9, 0, 255, 0);
            RGB_MATRIX_INDICATOR_SET_COLOR(10, 0, 255, 0);
            break;
        case 1:
            RGB_MATRIX_INDICATOR_SET_COLOR(9, 255, 0, 0);
            RGB_MATRIX_INDICATOR_SET_COLOR(10, 255, 0, 0);
            break;
        default:
            RGB_MATRIX_INDICATOR_SET_COLOR(9, 255, 255, 255);
            RGB_MATRIX_INDICATOR_SET_COLOR(10, 255, 255, 255);
            break;
    }
    return false;
}

// RGB underglow for layer state
layer_state_t layer_state_set_user(layer_state_t state) {
    //rgb_matrix_mode(RGB_MATRIX_NONE);
    switch (get_highest_layer(state)) {
    case 3:
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        rgb_matrix_sethsv_noeeprom(HSV_SPRINGGREEN);
        break;
    case 2:
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        rgb_matrix_sethsv_noeeprom(HSV_ORANGE);
        //rgb_matrix_mode(RGB_MATRIX_PIXEL_RAIN);
        break;
    case 1:
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        rgb_matrix_sethsv_noeeprom(HSV_PURPLE);

        break;
    default: //  for any other layers, or the default layer
        //rgb_matrix_mode(RGB_MATRIX_NONE);
        //rgb_matrix_set_color_all (0xFF,  0xFF, 0xFF);
	    //rgb_matrix_mode(RGB_MATRIX_CYCLE_ALL);
        rgb_matrix_mode_noeeprom(RGB_MATRIX_PIXEL_RAIN);
        //override for the underglow - set in rgb_matrix_indicators_advanced_user above
        break;
    }
  return state;
};

// set on-boot layer and lighting mode:
void keyboard_post_init_user(void) {
    // Force the keyboard to start on Layer 0
    layer_move(0);

    // Force the RGB matrix into Pixel Rain on boot without overwriting EEPROM
    rgb_matrix_mode_noeeprom(RGB_MATRIX_PIXEL_RAIN);
}

// RGB Lighting state for switching to bootloader
bool shutdown_user(bool jump_to_bootloader) {
    if (jump_to_bootloader) {
        // CHANGE KEY BACKLIGHTING
        // these don't work because the "frame" doesn't advance, so these never get set
        // rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        // rgb_matrix_sethsv_noeeprom(HSV_WHITE);

        for (int i = 0; i < RGB_MATRIX_LED_COUNT-2; i++) {
            rgb_matrix_set_color(i, 50, 50, 50);
        }
        // CHANGE INDICATOR LIGHTING
        // RGB_MATRIX_INDICATOR_SET_COLOR(9, 50, 50, 50);
        // the above function doesn't work outside of rgb_matrix_indicators_advanced_user
        // manually set them directly:
        rgb_matrix_set_color(9, 10, 10, 10);
        rgb_matrix_set_color(10, 10, 10, 10);
        // FORCE the RGB Matrix driver to push these values to the LEDs IMMEDIATELY
        rgb_matrix_update_pwm_buffers();
        // Disables the RGB matrix cleanly without saving the state
        // rgb_matrix_disable_noeeprom(); 
    }
    return true; // Return true to allow the shutdown sequence to continue
}

// Blocking out original encoder functionality
// Keeping it for reference
/*
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == _MIDDLE) {
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
*/
