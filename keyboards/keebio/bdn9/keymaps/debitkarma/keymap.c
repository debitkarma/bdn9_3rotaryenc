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
        ENCODER_CCW_CW(KC_F19, KC_F20),
        ENCODER_CCW_CW(KC_1, KC_2),
        ENCODER_CCW_CW(KC_4, KC_5),
    },
    [2] = {
        ENCODER_CCW_CW(KC_F19, KC_F20),
        ENCODER_CCW_CW(KC_1, KC_2),
        ENCODER_CCW_CW(KC_4, KC_5),
    },
    [3] = {
        ENCODER_CCW_CW(KC_F19, KC_F20),
        ENCODER_CCW_CW(KC_1, KC_2),
        ENCODER_CCW_CW(KC_4, KC_5),
    },
};
#endif

// Reset to zero whenever tapping a lot in a row
void reset_to_zero(tap_dance_state_t *state, void *user_data) {
  if (state->count >= 3) {
    // Reset the keyboard to Layer 0 if more than 3 taps happen on a tapdance key
    reset_tap_dance(state);
  }
}

// Tap Dance functions
// defining tap dance actions prior to keymap
tap_dance_action_t tap_dance_actions[] = {
  [TD_L1] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F13, 1),
  [TD_L2] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F14, 2),
  [TD_L3] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F15, 3),
  [L1_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F13, 0),
  [L2_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F14, 0),
  [L3_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F15, 0),
  [TD_RESET] = ACTION_TAP_DANCE_FN(reset_to_zero)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Press: Mute    | Press: Home | Press: End   |
        |  F13           | F14         | F15          |
        |  F16           | F17         | F18          |
     */
    [0] = LAYOUT(
        KC_MUTE  , KC_HOME  , KC_END,
        TD(TD_L1), TD(TD_L2), TD(TD_L3),
        KC_F16   , KC_F17   , KC_F18
    ),
    /*
        | Press: F21     | Press: 3     | Press: 6     |
        |  F13           | F14          | F15          |
        |  F16           | F17          | F18          |
     */
    [1] = LAYOUT(
        KC_F21       , KC_3   , KC_6  ,
        TD(L1_RESET) , KC_F14 , KC_F15,
        KC_F16       , KC_F17 , KC_F18
    ),
    /*
        | Press: F21     | Press: 3     | Press: 6     |
        |  F13           | F14          | F15          |
        |  F16           | F17          | F18          |
     */
    [2] = LAYOUT(
        KC_F21 , KC_3         , KC_6  ,
        KC_F13 , TD(L2_RESET) , KC_F15,
        KC_F16 , KC_F17       , KC_F18
    ),
    /*
        | Press: F21     | Press: 3     | Press: 6     |
        |  F13           | F14          | F15          |
        |  F16           | F17          | F18          |
     */
    [3] = LAYOUT(
        KC_F21 , KC_3   , KC_6  ,
        KC_F13 , KC_F14 , TD(L3_RESET),
        KC_F16 , KC_F17 , KC_F18
    ),
};

// F-key wrapping functionality
// On key down
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_F13 ... KC_F21:
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
            register_code(KC_F22);
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
    case KC_F13 ... KC_F21:
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
                    unregister_code(KC_F22);
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


// RGB underglow for layer state
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 3:
        rgb_matrix_set_color_all (0xFF,  0x00, 0xAC);
        break;
    case 2:
        rgb_matrix_set_color_all (0xAC,  0xFF, 0x00);
        break;
    case 1:
        rgb_matrix_set_color_all (0x00,  0xAC, 0xFF);
        break;
    default: //  for any other layers, or the default layer
        rgb_matrix_set_color_all (0xFF,  0xFF, 0xFF);
	//rgb_matrix_mode(RGB_MATRIX_CYCLE_ALL);
        //override for the underglow
        rgb_matrix_set_color(9, 0x00, 0xFF, 0x00);
        rgb_matrix_set_color(10, 0x00, 0x00, 0xFF);
        break;
    }
  return state;
};

// Reset to zero whenever tapping a lot in a row
/*void reset_to_zero(tap_dance_state_t *state, void *user_data) {
  if (state->count >= 3) {
    // Reset the keyboard to Layer 0 if more than 3 taps happen on a tapdance key
    reset_tap_dance(state);
  }
}*/

// Tap Dance functions
/*qk_tap_dance_action_t tap_dance_actions[] = {
  [TO_L1] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F13, 1),
  [TO_L2] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F14, 2),
  [TO_L3] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F15, 3),
  [L1_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F13, 0),
  [L2_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F14, 0),
  [L3_RESET] = ACTION_TAP_DANCE_LAYER_MOVE(KC_F15, 0),
  [TD_RESET] = ACTION_TAP_DANCE_FN(reset_to_zero)
};*/

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
