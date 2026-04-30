/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#include "quantum.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    RCTL_T(KC_A), KC_S  , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_COLN  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    MO(3)    , KC_RCTL  , KC_LOPT  , KC_LCMD  , LT(2,KC_SPC),KC_LSFT,        KC_BSPC,LT(1,KC_ENT),XXXXXXX,XXXXXXX,XXXXXXX, KC_ESC
  ),

  [1] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    KC_F11   ,KC_F12    , KC_TAB   ,_______   , _______  ,                            _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [2] = LAYOUT_universal(
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_QUES  , KC_BSLS  ,
    KC_LBRC  , KC_LCBR  , KC_LPRN  , KC_LT    , KC_MINS  ,                            KC_PLUS  , KC_GT    , KC_RPRN  , KC_RCBR  , KC_RBRC  ,
    KC_GRV   , KC_TILD  , KC_QUOT  , KC_DQUO  , KC_UNDS  ,                            KC_EQL   , KC_PIPE  , KC_SCLN  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    KC_KP_ASTERISK  , KC_KP_7  , KC_KP_8  , KC_KP_9  , KC_KP_MINUS  ,                     _______  , KC_BTN1  , KC_BTN3  , KC_BTN2  , _______ ,
    KC_KP_SLASH , KC_KP_4  , KC_KP_5  , KC_KP_6  , KC_KP_PLUS  ,                      _______  , KC_HOME  , KC_UP    , KC_END   , _______ ,
    KC_KP_0     , KC_KP_1  , KC_KP_2  , KC_KP_3  , KC_KP_DOT   ,                      _______  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______ ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      KC_BTN4  , KC_BTN5  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the middle layer is 2
    keyball_set_scroll_mode(get_highest_layer(state) == 2);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
