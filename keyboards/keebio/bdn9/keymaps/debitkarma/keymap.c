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
static uint8 f22_tracker;
static uint8 f23_tracker;
static uint8 f24_tracker;

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Press: Mute    | Press: Home | Press: End   |
        |  F13           | F14         | F15          |
        |  F16           | F17         | F18          |
     */
    [0] = LAYOUT(
        KC_MUTE, KC_HOME, KC_END,
        KC_F13 , KC_F14 , KC_F15,
        KC_F16 , KC_F17 , KC_F18
    ),
    /*
        | Press: F21     | Press: 3     | Press: 6     |
        |  F13           | F14          | F15          |
        |  F16           | F17          | F18          |
     */
    [1] = LAYOUT(
        KC_F21 , KC_3  , KC_6  ,
        KC_F13 , KC_F14 , KC_F15,
        KC_F16 , KC_F17 , KC_F18
    ),
    /*
        | Press: F21     | Press: 3     | Press: 6     |
        |  F13           | F14          | F15          |
        |  F16           | F17          | F18          |
     */
    [2] = LAYOUT(
        KC_F21 , KC_3  , KC_6  ,
        KC_F13 , KC_F14 , KC_F15,
        KC_F16 , KC_F17 , KC_F18
    ),
    /*
        | Press: F21     | Press: 3     | Press: 6     |
        |  F13           | F14          | F15          |
        |  F16           | F17          | F18          |
     */
    [3] = LAYOUT(
        KC_F21 , KC_3  , KC_6  ,
        KC_F13 , KC_F14 , KC_F15,
        KC_F16 , KC_F17 , KC_F18
    ),
};

// F-key wrapping functionality
// On key down
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_F13 ... KC_F21:
        switch (get_highest_layer(state)) {
        case 3:
            if (record->event.pressed) {
            register_code(KC_F24);
            f24_tracker++;
            register_code(keycode);
            return false;
            }
            break;
        case 2:
            if (record->event.pressed) {
            register_code(KC_F23);
            f23_tracker++;
            register_code(keycode);
            return false;
            }
            break;
        case 1:
            if (record->event.pressed) {
            register_code(KC_F22);
            f22_tracker++;
            register_code(keycode);
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
        switch (get_highest_layer(state)) {
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

// RGB underglow for layer state
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 3:
        rgblight_setrgb (0xFF,  0x00, 0xAC);
        break;
    case 2:
        rgblight_setrgb (0xAC,  0xFF, 0x00);
        break;
    case 1:
        rgblight_setrgb (0x00,  0xAC, 0xFF);
        break;
    default: //  for any other layers, or the default layer
        rgblight_setrgb (0xFF,  0xFF, 0xFF);
        break;
    }
  return state;
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
