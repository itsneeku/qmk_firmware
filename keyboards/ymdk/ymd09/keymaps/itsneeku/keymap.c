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
  BHOP
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(MOUSE_JIGGLE, KC_F13, KC_F14,
               KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK,
               KC_F15, KC_F16, KC_F17),
};

bool mouse_jiggle_mode = false;
uint32_t mouse_jiggle_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Stop mouse jiggle whenever ANY key is pressed
  if (record->event.pressed && keycode != MOUSE_JIGGLE) {
    mouse_jiggle_mode = false;
  }
  switch (keycode) {
    case MOUSE_JIGGLE:
      if (record->event.pressed) {
        mouse_jiggle_mode = !mouse_jiggle_mode;
        mouse_jiggle_timer = timer_read();
        break;
      }
    default:
      break;
  }
  return true; // Process all other keycodes normally
}

void matrix_scan_user(void) {
  if (mouse_jiggle_mode) {
    if (timer_elapsed32(mouse_jiggle_timer) > 60000 ) { 
      //60000ms = 1 minute
      tap_code(KC_MS_LEFT);
      tap_code(KC_MS_RIGHT);
      tap_code(KC_MS_WH_UP);
      tap_code(KC_MS_WH_DOWN);
      mouse_jiggle_timer = timer_read32();
    }
  }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  if (mouse_jiggle_mode) {
    RGB_MATRIX_INDICATOR_SET_COLOR(2, 250, 168, 178);
  } else {
    RGB_MATRIX_INDICATOR_SET_COLOR(2, 0, 0, 0);
  }
  return true;
}
