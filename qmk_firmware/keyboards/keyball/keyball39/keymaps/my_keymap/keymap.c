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
// 「論理的に同じ操作」を実現する。
//////////////////////////////////////////////////////////////////////////////

enum my_keycodes {
    U_SSHOT = KEYBALL_SAFE_RANGE, // 範囲スクショ : Mac=Cmd+Shift+4/ Win=Win+Shift+S
    U_SSFULL, // 全画面スクショ: Mac=Cmd+Shift+3/ Win=PrintScreen
    U_ZIN,    // 拡大         : Mac=Cmd+=      / Win=Ctrl+=
    U_ZOUT,   // 縮小         : Mac=Cmd+-      / Win=Ctrl+-
    U_ZRST,   // 100%         : Mac=Cmd+0      / Win=Ctrl+0
};

typedef enum { OS_WIN = 0, OS_MAC } my_os_t;
static my_os_t g_os = OS_WIN;

// 拡大縮小のプライマリ修飾: Mac は Cmd(GUI)、Win は Ctrl
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
    // Mac では左下の Win/Alt を入れ替え、中指を Cmd にする
    // （Mac の Cmd+Tab と Win の Alt+Tab を同じ指で打てるように）
    keymap_config.swap_lalt_lgui = (g_os == OS_MAC);
    return true;
}
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // [0] ベース: 英字・Space・Enter には hold/tap を付けない。Mac では Win(薬指)=Option、Alt(中指)=Cmd
  // 親指の Shift はワンショット: タップして離すと次の1キーに Shift が付く（Shift をタップ → Ctrl+T で Ctrl+Shift+T）
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_COLN  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    LT(3,KC_ESC), KC_LGUI, KC_LALT , OSM(MOD_RSFT), MO(1)    , KC_LCTL  ,     LT(2,KC_BSPC), KC_SPC, XXXXXXX, XXXXXXX, XXXXXXX, KC_ENT
  ),

  // [1] 記号（左親指。押している間スクロールモード）
  [1] = LAYOUT_universal(
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_QUES  , KC_BSLS  ,
    KC_LBRC  , KC_LCBR  , KC_LPRN  , KC_LT    , KC_MINS  ,                            KC_PLUS  , KC_GT    , KC_RPRN  , KC_RCBR  , KC_RBRC  ,
    KC_GRV   , KC_TILD  , KC_QUOT  , KC_DQUO  , KC_UNDS  ,                            KC_EQL   , KC_PIPE  , KC_SCLN  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  // [2] 数字・F キー（右親指 BSpc 長押し）
  // 数字はホーム行に普通のキーボードと同じ列で並べ、F キーは同じ番号の数字の真上に置く
  // Shift と Tab を隣り合わせにして Shift+Tab を片手で打てるようにする。下段は透過（Ctrl+Tab・Alt+Tab 用）
  [2] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    KC_LSFT  , KC_TAB   , KC_F11   , KC_F12   , XXXXXXX  ,                            KC_MINS  , XXXXXXX  , KC_COMM  , KC_DOT   , KC_SLSH  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  // [3] マウス・矢印（左小指 Esc 長押し）。左手は小指で押さえるので下段だけ使う
  [3] = LAYOUT_universal(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            KC_VOLU  , KC_BTN1  , KC_BTN3  , KC_BTN2  , U_ZIN    ,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            KC_VOLD  , KC_HOME  , KC_UP    , KC_END   , U_ZOUT   ,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , KC_LEFT  , KC_DOWN  , KC_RGHT  , U_ZRST   ,
    _______  , U_SSHOT  , U_SSFULL , _______  , XXXXXXX  , _______  ,      KC_BTN4  , KC_BTN5  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),
};
// clang-format on

//////////////////////////////////////////////////////////////////////////////
// 論理キーコードの処理

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
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

        case U_ZIN:  tap_with_mod(primary_mod(), KC_EQL);  return false;
        case U_ZOUT: tap_with_mod(primary_mod(), KC_MINS); return false;
        case U_ZRST: tap_with_mod(primary_mod(), KC_0);    return false;
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

// 別キーを「押して離した」ときだけ hold にする。
// BSpc/L2 だけに付け、数字を速く打っても BSpc に化けないようにする。
// Esc/L3 には付けない（Vim で Esc の直後に重ねて打ったキーが L3 に化けないように）
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(2, KC_BSPC):
            return true;
        default:
            return false;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // 記号レイヤー(1)を押している間はボールでスクロール
    keyball_set_scroll_mode(get_highest_layer(state) == 1);
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
