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

//////////////////////////////////////////////////////////////////////////////
// OS 差を吸収する論理キーコード
//
// Mac / Win を os_detection で自動判別し、同じ物理キーで
// 「論理的に同じ操作」を実現する。U_OS で手動上書きも可能。
//////////////////////////////////////////////////////////////////////////////

enum my_keycodes {
    U_OS = KEYBALL_SAFE_RANGE, // OS 手動トグル（自動判定の上書き用）
    U_CPY,    // コピー       : Mac=Cmd+C      / Win=Ctrl+C
    U_PST,    // ペースト     : Mac=Cmd+V      / Win=Ctrl+V
    U_CUT,    // カット       : Mac=Cmd+X      / Win=Ctrl+X
    U_UNDO,   // 元に戻す     : Mac=Cmd+Z      / Win=Ctrl+Z
    U_REDO,   // やり直し     : Mac=Cmd+Shift+Z/ Win=Ctrl+Y
    U_SALL,   // 全選択       : Mac=Cmd+A      / Win=Ctrl+A
    U_SAVE,   // 保存         : Mac=Cmd+S      / Win=Ctrl+S
    U_FIND,   // 検索         : Mac=Cmd+F      / Win=Ctrl+F
    U_SSHOT,  // 範囲スクショ : Mac=Cmd+Shift+4/ Win=Win+Shift+S
    U_SSFULL, // 全画面スクショ: Mac=Cmd+Shift+3/ Win=PrintScreen
    U_WL,     // 単語左移動   : Mac=Opt+Left   / Win=Ctrl+Left
    U_WR,     // 単語右移動   : Mac=Opt+Right  / Win=Ctrl+Right
    U_LNL,    // 行頭         : Mac=Cmd+Left   / Win=Home
    U_LNR,    // 行末         : Mac=Cmd+Right  / Win=End
};

typedef enum { OS_WIN = 0, OS_MAC } my_os_t;
static my_os_t g_os = OS_WIN;

// コピペ等のプライマリ修飾: Mac は Cmd(GUI)、Win は Ctrl
static inline uint16_t primary_mod(void) {
    return g_os == OS_MAC ? KC_LGUI : KC_LCTL;
}

// mod を押しながら code を1回叩く
static void tap_with_mod(uint16_t mod, uint16_t code) {
    register_code16(mod);
    tap_code16(code);
    unregister_code16(mod);
}

#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h" // os_variant_t / OS_MACOS などの定義

// os_detection が OS を確定したときに呼ばれる
bool process_detected_host_os_user(os_variant_t os) {
    g_os = (os == OS_MACOS || os == OS_IOS) ? OS_MAC : OS_WIN;
    return true;
}
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // [0] ベース: 英字・記号は Mac/Win 共通。親指の修飾は GUI(=Cmd/Win) に統一
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    LCTL_T(KC_A), KC_S  , LSFT_T(KC_D), KC_F   , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_COLN  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    MO(3)    , KC_LGUI  , KC_LALT  , KC_RSFT  , LT(2,KC_SPC), KC_LCTL  ,     KC_BSPC, LT(1,KC_ENT), XXXXXXX, XXXXXXX, XXXXXXX, KC_ESC
  ),

  // [1] 数字・ファンクション + OS対応の編集/スクショ/OSトグル
  [1] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    KC_F11   , KC_F12   , KC_TAB   , U_SSHOT  , U_SSFULL ,                            U_UNDO   , U_REDO   , U_CUT    , U_CPY    , U_PST    ,
    _______  , U_OS     , U_SALL   , U_SAVE   , U_FIND   , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  // [2] 記号（押している間スクロールモード）
  [2] = LAYOUT_universal(
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_QUES  , KC_BSLS  ,
    KC_LBRC  , KC_LCBR  , KC_LPRN  , KC_LT    , KC_MINS  ,                            KC_PLUS  , KC_GT    , KC_RPRN  , KC_RCBR  , KC_RBRC  ,
    KC_GRV   , KC_TILD  , KC_QUOT  , KC_DQUO  , KC_UNDS  ,                            KC_EQL   , KC_PIPE  , KC_SCLN  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  // [3] テンキー・マウス・矢印
  [3] = LAYOUT_universal(
    KC_KP_ASTERISK  , KC_KP_7  , KC_KP_8  , KC_KP_9  , KC_KP_MINUS  ,                     _______  , KC_BTN1  , KC_BTN3  , KC_BTN2  , _______ ,
    KC_KP_SLASH , KC_KP_4  , KC_KP_5  , KC_KP_6  , KC_KP_PLUS  ,                      _______  , KC_HOME  , KC_UP    , KC_END   , _______ ,
    KC_KP_0     , KC_KP_1  , KC_KP_2  , KC_KP_3  , KC_KP_DOT   ,                      _______  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______ ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      KC_BTN4  , KC_BTN5  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

//////////////////////////////////////////////////////////////////////////////
// 論理キーコードの処理

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    const uint16_t p = primary_mod();
    switch (keycode) {
        case U_OS:
            g_os = (g_os == OS_MAC) ? OS_WIN : OS_MAC;
            return false;

        case U_CPY:  tap_with_mod(p, KC_C); return false;
        case U_PST:  tap_with_mod(p, KC_V); return false;
        case U_CUT:  tap_with_mod(p, KC_X); return false;
        case U_SALL: tap_with_mod(p, KC_A); return false;
        case U_SAVE: tap_with_mod(p, KC_S); return false;
        case U_FIND: tap_with_mod(p, KC_F); return false;
        case U_UNDO: tap_with_mod(p, KC_Z); return false;

        case U_REDO:
            if (g_os == OS_MAC) {
                register_code(KC_LSFT);
                tap_with_mod(KC_LGUI, KC_Z);
                unregister_code(KC_LSFT);
            } else {
                tap_with_mod(KC_LCTL, KC_Y);
            }
            return false;

        case U_SSHOT: // 範囲スクショ
            register_code(KC_LSFT);
            tap_with_mod(KC_LGUI, g_os == OS_MAC ? KC_4 : KC_S);
            unregister_code(KC_LSFT);
            return false;

        case U_SSFULL: // 全画面スクショ
            if (g_os == OS_MAC) {
                register_code(KC_LSFT);
                tap_with_mod(KC_LGUI, KC_3);
                unregister_code(KC_LSFT);
            } else {
                tap_code(KC_PSCR);
            }
            return false;

        case U_WL: tap_with_mod(g_os == OS_MAC ? KC_LALT : KC_LCTL, KC_LEFT);  return false;
        case U_WR: tap_with_mod(g_os == OS_MAC ? KC_LALT : KC_LCTL, KC_RIGHT); return false;
        case U_LNL: if (g_os == OS_MAC) { tap_with_mod(KC_LGUI, KC_LEFT); }  else { tap_code(KC_HOME); } return false;
        case U_LNR: if (g_os == OS_MAC) { tap_with_mod(KC_LGUI, KC_RIGHT); } else { tap_code(KC_END); }  return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////
// Caps Lock 封じ込め: ホストが Caps ON を報告したら即座に打ち消す
// （このキーマップは KC_CAPS を一切出さないので、他マシンから持ち込んだ
//   Caps ON 状態を自動で解除する用途）

void housekeeping_task_user(void) {
    if (host_keyboard_led_state().caps_lock) {
        tap_code(KC_CAPS);
    }
}

//////////////////////////////////////////////////////////////////////////////
// Tap/Hold チューニング

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCTL_T(KC_A):
            return 300;  // Ctrl誤爆防止: 長く保持しないとCtrlにならない
        case LSFT_T(KC_D):
        case LT(2, KC_SPC):
            return 150;  // Shift/Layer2: 短時間のholdでも発動するように
        default:
            return TAPPING_TERM;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCTL_T(KC_A):
            return false;  // ローリング中は Ctrl にしない（tap優先）
        case LSFT_T(KC_D):
        case LT(2, KC_SPC):
            return true;   // 別キーを押した瞬間に Shift/Layer2 確定
        default:
            return false;
    }
}

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
