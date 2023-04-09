/* Copyright 2021 @ Keychron (https://www.keychron.com)
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
#include <time.h>
// clang-format off

enum layers{
    WIN_BASE,
    WIN_FN
};

enum custom_keycodes {
  MOUSE_JIGGLE = SAFE_RANGE,
  BHOP
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_DEL,             KC_PRINT_SCREEN,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_F14,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_F13,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_MEDIA_PLAY_PAUSE,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              MO(WIN_FN),  KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 MOUSE_JIGGLE,  QK_LOCK, KC_F15,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  LGUI(KC_TAB),  LGUI(KC_E),  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            KC_F20,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            KC_F19,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            KC_F18,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            KC_F17,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
};

//https://www.reddit.com/r/olkb/comments/nfbai5/i_got_macros_to_rapid_fire_while_held_down_using/

bool mouse_jiggle_mode = false;
uint32_t mouse_jiggle_timer = 0;
bool is_last_key_qk_lock = false;
bool qk_lock_mode = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Stop mouse jiggle whenever ANY key is pressed
  if (record->event.pressed) {
    mouse_jiggle_mode = false;
  }
  switch (keycode) {
    case MOUSE_JIGGLE:
      if (record->event.pressed) {
        mouse_jiggle_mode = !mouse_jiggle_mode;
        mouse_jiggle_timer = timer_read();
        break;
      }
    case QK_LOCK:
      if (record->event.pressed) {
        is_last_key_qk_lock = true;
        qk_lock_mode = true;
        break;
      }
    default:
      if (record->event.pressed) {
        if (!is_last_key_qk_lock && qk_lock_mode){
          qk_lock_mode = false;
        }
        else if (is_last_key_qk_lock) {
          is_last_key_qk_lock = false;
        }
      }
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
    RGB_MATRIX_INDICATOR_SET_COLOR(MOUSE_JIGGLE_LED_INDEX, 255, 255, 255);
  } else {
    RGB_MATRIX_INDICATOR_SET_COLOR(MOUSE_JIGGLE_LED_INDEX, 0, 0, 0);
  }
  // if (qk_lock_mode) {
  //   RGB_MATRIX_INDICATOR_SET_COLOR(QK_LOCK_LED_INDEX, 255, 255, 255);
  // } else {
  //   RGB_MATRIX_INDICATOR_SET_COLOR(QK_LOCK_LED_INDEX, 0, 0, 0);
  // }

  return true;
}
