/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

uint16_t copy_paste_timer;

enum layers {
    COLEMAK = 0,
    RAISE,
    SYMBOLS,
    ADJUST
};

enum custom_keycodes {
    KC_CCCV = SAFE_RANGE,
    CTLSFT_LEAD,
    ENC_ALTTAB_CW,
    ENC_ALTTAB_CCW,
    ENC_CTRLTAB_CW,
    ENC_CTRLTAB_CCW,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: COLEMAK
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | ESC    |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | LSFT   |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LCTL   |   Z  |   X  |   C  |   V  |   B  | CCCV |      |  | Del  |Leader|   N  |   M  | ,  < | . >  | /  ? |  - _   |
 * `----------------------+------+------+------+------+      |  |------+------+------+------+------+----------------------'
 *                        | GUI  | Alt  |      | Space| Enter|  | Bspc | Space|      | Tab  | AltGr|
 *                        |      |      | RAISE| Shift| Alt  |  |      | Nav  | SYMBOLS|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [COLEMAK] = LAYOUT(
             KC_GRV,  KC_Q,   KC_W,   KC_F,   KC_P,   KC_G,                                           KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_EQL,
LT(SYMBOLS, KC_LGUI), KC_A,   KC_R,   KC_S,   KC_T,   KC_D,                                           KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
MT(MOD_LSFT, KC_TAB), KC_Z,   KC_X,   KC_C,   KC_V,   KC_B, CTLSFT_LEAD, KC_ESC,     KC_PGUP,KC_PGDN, KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_MINS,
KC_ENT, KC_LALT, LT(RAISE, KC_BSPC), MT(MOD_LSFT, KC_ENT), MT(MOD_LCTL, KC_TAB),     MT(MOD_LCTL, KC_TAB), MT(MOD_LSFT, KC_ENT), LT(RAISE, KC_SPC), LT(SYMBOLS, KC_DEL), KC_MPLY
    ),
/*
 * RAISE Layer: Numpad, Media
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      | VolUp|      |      |                              | / ?  | 7 &  | 8 *  | 9 (  | - _  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next |      |                              | *    | 4 $  | 5 %  | 6 ^  | , <  | +      |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      | VolDn| Mute |      |      |      |  |      |      | 0 )  | 1 !  | 2 @  | 3 #  | = +  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | RAISE|      |      |  |      | Nav  | SYMBOLS|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [RAISE] = LAYOUT(
      _______, KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                                                          KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_PIPE,
      KC_EXLM, RALT(KC_A), RALT(KC_R), KC_HOME,    KC_END,     RALT(KC_D),                                                    RALT(KC_U), RALT(KC_N), RALT(KC_E), RALT(KC_I), RALT(KC_O), RALT(KC_QUOT),
      UG_TOGG, RALT(KC_Z), RALT(KC_X), RALT(KC_C), RALT(KC_V), RALT(KC_P), _______, _______,          KC_PGUP,     KC_PGDN,  _______,   KC_LEFT,    KC_UP,      KC_DOWN,    KC_RGHT,    KC_BSLS,
                                 _______, _______, _______, _______, _______,                         LCTL(KC_LEFT), LCTL(KC_RGHT), _______, _______, _______
    ),
/*
 * SYMBOLS Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  @   |  {   |  }   |  |   |                              |      |  _   |  €   |      |      |  \     |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  #   |  $   |  (   |  )   |  `   |                              |   +  |  -   |  /   |  *   |  %   |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  %   |  ^   |  [   |  ]   |  ~   |      |      |  |      |      |   &  |  =   |  ,   |  .   |  / ? | - _    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | RAISE|      |      |  |      | Nav  | SYMBOLS|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [SYMBOLS] = LAYOUT(
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                     KC_CIRC, KC_F1, KC_F2,  KC_F3,  KC_F4, _______,
      _______, KC_EXLM, KC_LPRN, KC_LBRC, KC_LCBR, ALGR(KC_G),                                  _______, KC_F5, KC_F6,  KC_F7,  KC_F8, _______,
      KC_CIRC, KC_ASTR, KC_RPRN, KC_RBRC, KC_RCBR, KC_AMPR, _______, _______, _______, _______, _______, KC_F9, KC_F10, KC_F11, KC_F12, _______,
                                 KC_MUTE, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      | F7   | F8   | F9   | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | SAI  | HUI  | VAI  |      |                              |      | F4   | F5   | F6   | F11  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  |      |      |      |  |      |      |      | F1   | F2   | F3   | F12  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [ADJUST] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      _______, UG_SPDU, UG_SATU, UG_HUEU, UG_VALU, UG_NEXT,                                     _______, KC_F4,   KC_F5,   KC_F6,   KC_F11,  _______,
      _______, UG_SPDD, UG_SATD, UG_HUED, UG_VALD, UG_PREV, _______, _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F12,  _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

// holding both RAISE and SYMBOLS enables ADJUST layer
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, RAISE, SYMBOLS, ADJUST);
}

// LEADER config starts here
void leader_end_user(void) {
    if (leader_sequence_one_key(KC_K)) {
        // IJ git push
        SEND_STRING(SS_LCTL(SS_LSFT("K")));
    } else if (leader_sequence_one_key(KC_F)) {
        // IJ find in path
        SEND_STRING(SS_LCTL(SS_LSFT("F")));
    } else if (leader_sequence_one_key(KC_U)) {
        // IJ toggle upper/lower case
        SEND_STRING(SS_LCTL(SS_LSFT("U")));
    } else if (leader_sequence_one_key(KC_P)) {
        // VScode command palette
        SEND_STRING(SS_LCTL(SS_LSFT("P")));
    } else if (leader_sequence_one_key(KC_I)) {
        // FF debugger
        SEND_STRING(SS_LCTL(SS_LSFT("I")));
    } else if (leader_sequence_one_key(KC_E)) {
        // email
        SEND_STRING("florent@nicoulaud.org");
    } else if (leader_sequence_one_key(KC_N)) {
        // email presta
        SEND_STRING("florent.nicoulaud-prestataire@labanquepostale.fr");
    }
}

// #if defined(ENCODER_MAP_ENABLE)
// const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
//     [0] = { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
//     [1] = { ENCODER_CCW_CW(UG_HUED, UG_HUEU),  ENCODER_CCW_CW(UG_SATD, UG_SATU)  },
//     [2] = { ENCODER_CCW_CW(UG_VALD, UG_VALU),  ENCODER_CCW_CW(UG_SPDD, UG_SPDU)  },
//     [3] = { ENCODER_CCW_CW(UG_PREV, UG_NEXT),  ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
// };
// #endif


// ROTARY encoders starts here
#ifdef ENCODER_ENABLE

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [COLEMAK] = { ENCODER_CCW_CW(ENC_ALTTAB_CW,  ENC_ALTTAB_CCW),  ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
    [RAISE]   = { ENCODER_CCW_CW(ENC_CTRLTAB_CW, ENC_CTRLTAB_CCW), ENCODER_CCW_CW(C(KC_RIGHT), C(KC_LEFT)) },
    [SYMBOLS] = { ENCODER_CCW_CW(KC_BRIU, KC_BRID),                ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
    [ADJUST]  = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),                ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
};
#endif

// ROTARY encoders variables
uint16_t cs_lead_timer;
bool is_alt_tab_active = false;
uint16_t alt_tab_timer;
#define TABBING_TIMER 1000

void matrix_scan_user(void) {
    // this is for ROTARY alt tab
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > TABBING_TIMER) {
          unregister_code(KC_LALT);
          unregister_code(KC_LCTL);
          is_alt_tab_active = false;
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_CCCV:  // One key copy/paste
            if (record->event.pressed) {
                copy_paste_timer = timer_read();
            } else {
                if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {  // Hold, copy
                    tap_code16(LCTL(KC_C));
                } else { // Tap, paste
                    tap_code16(LCTL(KC_V));
                }
            }
            break;
        case CTLSFT_LEAD:
            if (record->event.pressed) {
                cs_lead_timer = timer_read();
                register_code(KC_LCTL);
                register_code(KC_LSFT);
            } else {
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                if (timer_elapsed(cs_lead_timer) < TAPPING_TERM) {
                    leader_start();  // it was a tap, not a hold
                }
            }
            return false;

        case ENC_ALTTAB_CW:
        case ENC_ALTTAB_CCW:
            if (record->event.pressed) {
                alt_tab_timer = timer_read();
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                tap_code16(keycode == ENC_ALTTAB_CCW ? S(KC_TAB) : KC_TAB);
            }
            return false;

        case ENC_CTRLTAB_CW:
        case ENC_CTRLTAB_CCW:
            if (record->event.pressed) {
                alt_tab_timer = timer_read();
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LCTL);
                }
                tap_code16(keycode == ENC_CTRLTAB_CCW ? S(KC_TAB) : KC_TAB);
            }
            return false;
    }
    return true;
}
#endif


// OLED config starts here
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("         Kyria rev1.0"), false);

        // Host Keyboard Layer Status
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case COLEMAK:
                oled_write_P(PSTR("Colemak\n"), false);
                break;
            case RAISE:
                oled_write_P(PSTR("RAISE\n"), false);
                break;
            case SYMBOLS:
                oled_write_P(PSTR("SYMBOLS\n"), false);
                break;
            case ADJUST:
                oled_write_P(PSTR("ADJUST\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }
        oled_write_P(PSTR("\n2026 - r0m3o's build"), false);

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}
#endif
