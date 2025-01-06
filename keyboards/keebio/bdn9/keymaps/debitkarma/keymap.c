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
