/*
Copyright 2023
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <time.h>

enum custom_keycodes {
  MOUSE_JIGGLE = SAFE_RANGE,
  BHOP,
  SPAM_SCROLL_UP,
  SPAM_SCROLL_DOWN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(MOUSE_JIGGLE,        SPAM_SCROLL_UP,               SPAM_SCROLL_DOWN,
               KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE,  KC_MEDIA_NEXT_TRACK,
               KC_F13,              KC_F14,               KC_F15),
};

struct spam_flag {
    uint16_t key_codes[4];
    bool     enabled;
    uint32_t timer;
    int      delay;
    int      rgb_matrix_index;
};

struct spam_flag spam_modes[] = {
    // MOUSE_JIGGLE
    {{KC_MS_LEFT, KC_MS_RIGHT, KC_MS_WH_UP, KC_MS_WH_DOWN}, false, 0, 60000, 2},

    // SPAM_SCROLL_UP
    {{KC_MS_WH_UP}, false, 0, 1000, 1},

    // SPAM_SCROLL_DOWN
    {{KC_MS_WH_DOWN}, false, 0, 1000, 0}};

void handle_spam(void) {
    for (int i = 0; i < sizeof(spam_modes) / sizeof(spam_modes[0]); i++) {
        if (spam_modes[i].enabled && timer_elapsed32(spam_modes[i].timer) > spam_modes[i].delay) {
            for (int j = 0; j < sizeof(spam_modes[i].key_codes) / sizeof(spam_modes[i].key_codes[0]); j++) {
                if (spam_modes[i].key_codes[j]) {
                    tap_code(spam_modes[i].key_codes[j]);
                    spam_modes[i].timer = timer_read32();
                }
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MOUSE_JIGGLE:
            if (record->event.pressed) {
                spam_modes[0].enabled = !spam_modes[0].enabled;
                break;
            }
        case SPAM_SCROLL_UP:
            if (record->event.pressed) {
                spam_modes[1].enabled = !spam_modes[1].enabled;
                break;
            }
        case SPAM_SCROLL_DOWN:
            if (record->event.pressed) {
                spam_modes[2].enabled = !spam_modes[2].enabled;
                break;
            }
        default:
            break;
    }
    return true; // Process all other keycodes normally
}

void matrix_scan_user(void) {
    handle_spam();
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (int i = 0; i < sizeof(spam_modes) / sizeof(spam_modes[0]); i++) {
        if (spam_modes[i].enabled) {
            RGB_MATRIX_INDICATOR_SET_COLOR(spam_modes[i].rgb_matrix_index, 250, 168, 178);
        } else {
            RGB_MATRIX_INDICATOR_SET_COLOR(spam_modes[i].rgb_matrix_index, 0, 0, 0);
        }
    }
    return true;
}