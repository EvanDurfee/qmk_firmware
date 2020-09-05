/* Copyright 2018 Yiancar
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

#define TT_FN_MAX_DURATION_MS 150

//This is the HHKB version of the PCB

enum my_layers {
  BASE_LAYER = 0,
  GAMING_LAYER,
  SPACE_FN_LAYER,
  // NAV_LAYER,
  FN_LAYER
};

#define TG_GAME TG(GAMING_LAYER)
#define KC_RWRD LCTL(KC_RGHT)
#define KC_LWRD LCTL(KC_LEFT)

enum my_keycodes {
    // Below uses process_record_user()
    TT_FN = SAFE_RANGE, // tap to toggle FN layer, hold for temporary activation
    // TT_NVFN // Tap for nav, hold for fn
};

static uint16_t key_timer;

/*
Ideas:

tap shifts for backspace / delete
tap shift / ctrl / alt for (, [, {
Autoshift / TapHold (e.g. tap fwo g, hold for G, tap for 1, hold for F1)
Dedicated symbols and numpad layer
keys for word right, word left, other helpful navigation
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE_LAYER] = LAYOUT_60_hhkb( /* Base */
      KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS,  KC_GRV,\
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,    KC_BSPC,    \
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   MT(MOD_RCTL, KC_ENT),\
      KC_LSFT,      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,        MT(LC_UP, KC_RSFT),       MO(FN_LAYER),  \
      KC_LCTL,   KC_LGUI, KC_LALT,                LT(SPACE_FN_LAYER, KC_SPC),                               MT(KC_LEFT, KC_RALT),   MT(KC_DOWN, KC_RGUI), MT(KC_RGHT, KC_RCTL)  ),

[SPACE_FN_LAYER] = LAYOUT_60_hhkb( /* Gaming stuff */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,    KC_F9,   KC_F10,  KC_F11,  KC_F12, KC_PSCR, KC_INS, \
    KC_CAPS,   _______, _______, KC_LCBR, KC_RCBR, _______, KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_PGDN, _______, _______,        KC_DEL, \
    _______,    _______, _______, KC_LPRN, KC_RPRN, _______, KC_LWRD, KC_LEFT, KC_DOWN, KC_RGHT, KC_RWRD,  _______,                _______,\
    _______,      _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______,  _______, _______, _______,        _______,      _______,\
    _______, _______, _______,                   _______,                                                  _______,   _______, _______  ),

[GAMING_LAYER] = LAYOUT_60_hhkb( /* Gaming stuff */
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,\
    _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______,\
    KC_CAPS,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                KC_ENT, \
    _______,      _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______,        _______,      _______,\
    _______, _______, _______,                   KC_SPC,                                                   _______,   _______, _______  ),

// TODO: add insert, print
[FN_LAYER] = LAYOUT_60_hhkb( /* FN */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,    KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______, _______,\
    KC_CAPS, EF_DEC,  EF_INC,  H1_DEC,  H1_INC,  H2_DEC,  H2_INC,  BR_DEC,  BR_INC,   ES_DEC,  ES_INC,   KC_UP,    _______,           KC_DEL, \
    _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, _______, _______, _______, KC_HOME,  KC_PGUP, KC_LEFT,  KC_RGHT,            _______,         \
    _______, TG_GAME, S1_DEC,  S1_INC,  S2_DEC,  S2_INC,  _______, _______,  KC_END,  KC_PGDN,  KC_DOWN,  _______,           _______,\
    _______, _______, _______,                 _______,                                        _______,  _______,            _______         ),
};

void matrix_init_user(void) {
  //user initialization
}

void matrix_scan_user(void) {
  //user matrix
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case TT_FN:
      if (record->event.pressed) {                  // Key Down, start timer and turn on the held layer
        key_timer = timer_read();
        if (IS_LAYER_ON(FN_LAYER)) {
          // If the layer is already active, toggle it off
          layer_off(FN_LAYER);
        }
        else {
          // Toggle on the layer
          layer_on(FN_LAYER);
        }
      }
      else {
        // Key released. If the layer was activated, test if this was a tap or hold
        if (IS_LAYER_ON(FN_LAYER)) {
          if (timer_elapsed(key_timer) < TT_FN_MAX_DURATION_MS) {
            // Treat this as a tap. Leave the layer on
          }
          else {
            // Treat this as a hold. Disable the layer
            layer_off(FN_LAYER);
          }
        }
      }
      break;
    // case TT_NVFN:
    //   if (record->event.pressed) {                  // Key Down, start timer and turn on the held layer
    //     key_timer = timer_read();
    //     layer_on(FN_LAYER);
    //     // if (IS_LAYER_ON(FN_LAYER)) {
    //     //   // If the layer is already active, toggle it off
    //     //   layer_off(FN_LAYER);
    //     // }
    //     // else {
    //     //   // Toggle on the layer
    //     //   layer_on(FN_LAYER);
    //     // }
    //   }
    //   else {
    //     layer_off(FN_LAYER);
    //     // Key released. If the layer was activated, test if this was a tap or hold
    //     if (timer_elapsed(key_timer) < TT_FN_MAX_DURATION_MS) {
    //       layer_invert(NAV_LAYER);
    //     }
    //     else {
    //       // Treat this as a hold. Ignore
    //     }
    //   }
    //   break;
  }
  return true;
}
