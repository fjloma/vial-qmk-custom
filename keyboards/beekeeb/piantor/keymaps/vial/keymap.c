// Copyright 2022 Diego Palacios (@diepala)
// SPDX-License-Identifier: GPL-2.0

#include QMK_KEYBOARD_H

#include "quantum/keymap_extras/keymap_spanish.h"
#include "quantum/keymap_extras/sendstring_spanish.h"
#include "quantum/keycodes.h"
#include "quantum/caps_word.h"
#include "quantum/backlight/backlight.h"


#include "features/layer_lock.h"
#include "features/chordmods.h"


enum layers {
	_COLEMAK,
	_NAV,
	_NUM,
	_MOUS, // NAV+NUM
	_SYMa,
	_SYMb,
	_QWER
};

#define ____ KC_TRNS
#define ESCM LT(_MOUS,KC_ESC)
#define LNAV TL_LOWR
#define LNUM TL_UPPR
#define LSYMa MO(_SYMa)
#define LSYMb MO(_SYMb)


enum custom_keycode {
    DIRBACK = QK_KB_0,
    CURRDIR,
    ARROW,
    GROOVY_DOLLAR,
    LLOCK
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*  0 COLEMAK DH
      * ┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
      * │EscM│ Q  │ W  │ F  │ P  │ B  │       │ J  │ L  │ U  │ Y  │Bsp │Del │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │Tab │ A  │ R  │ S  │ T  │ G  │       │ M  │ N  │ E  │ I  │ O  │Ent │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │GUI │ Z  │ X  │ C  │ D  │ V  │       │ N  │ M  │ ,  │ .  │ /  │Sft │
      * └────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
      *                ┌────┐                           ┌────┐
      *                │NUM ├────┐                 ┌────┤SYMa│
      *                └────┤NAV ├────┐       ┌────┤Spc ├────┘
      *                     └────┤Spc │       │Bsp ├────┘
      *                          └────┘       └────┘
      */
  	[_COLEMAK] = LAYOUT_split_3x6_3( \
		ESCM,		KC_Q,		KC_W,		KC_F,		KC_P,		KC_B, 				KC_J,		KC_L,		KC_U,		KC_Y,		KC_BSPC,	KC_DEL, \
		KC_TAB, 	KC_A, 		KC_R, 		KC_S, 		KC_T, 	    KC_G, 				KC_M,		KC_N, 		KC_E, 		KC_I, 		KC_O, 		KC_ENT, \
		KC_LGUI, 	KC_Z,		KC_X, 		KC_C, 		KC_D, 	    KC_V, 				KC_K, 		KC_H, 		KC_COMM, 	KC_DOT, 	KC_RSFT, 	KC_RCTL, \
											LNUM, 		LNAV, 	    KC_SPC, 			KC_BSPC, 	KC_SPC, 	LSYMa),

     /*
      *  3 NAV
      * ┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
      * │F16 │___ │⌘ Z │Del │⌘ V │___ │       │ ⇤  │PUp │ △  │PDo │ ⇥  │___ │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │___ │Ctr │Gui │Sft │Alt │___ │       │⌘◁  │ ◁  │ ▽  │ ▷  │⌘▷  │___ │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │Lock│⌘ Z │⌘ X │⌘ C │⌘ D │⌘ V │       │Mtc │ ⎇◁| ⎇▷ │ F3 │___ │___ │
      * └────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
      *                ┌────┐                           ┌────┐
      *                │___ ├────┐                 ┌────┤___ │
      *                └────┤___ ├────┐       ┌────┤___ ├────┘
      *                     └────┤___ │       │___ ├────┘
      *                          └────┘       └────┘
      */
	[_NAV] = LAYOUT_split_3x6_3( \
		KC_F16,	____,		LGUI(KC_Z),	KC_DEL,		LGUI(KC_V),	____,			KC_HOME,		KC_PGUP,	KC_UP,		KC_PGDN,	KC_END, 		____,  \
		____, 	KC_LCTL,	KC_LGUI,	KC_LSFT, 	KC_LALT, 	____, 			RGUI(KC_LEFT), 	KC_LEFT,	KC_DOWN,	KC_RGHT,	RGUI(KC_RGHT),	____, \
		LLOCK,	LGUI(KC_Z),	LGUI(KC_X),	LGUI(KC_C), LGUI(KC_D), LGUI(KC_V),		SGUI(KC_M), 	RALT(KC_LEFT), RALT(KC_RGHT), KC_F3, ____, 			____,  \
										____, 		____, 		____, 			____, 		____, 		____),

      /*
      *  4 NUM
      * ┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
      * │___ │F01 │F02 │F03 │F04 │___ │       │ ,  │ 7  │ 8  │ 9  │ -  │ /  │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │___ │F05 │F06 │F07 │F08 │ (  │       │ =  │ 4  │ 5  │ 6  │ +  │ *  │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │Lock│F09 │F10 │F11 │F12 │ )  │       │ 0  │ 1  │ 2  │ 3  │___ │___ │
      * └────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
      *                ┌────┐                           ┌────┐
      *                │___ ├────┐                 ┌────┤ .  │
      *                └────┤___ ├────┐       ┌────┤ 0  ├────┘
      *                     └────┤___ │       │___ ├────┘
      *                          └────┘       └────┘
      */
	[_NUM] = LAYOUT_split_3x6_3( \
		____, KC_F1,  KC_F2,  KC_F3,  KC_F4,  ____, 		KC_COMM,KC_7, KC_8, KC_9, KC_PMNS, KC_PSLS, \
		____, KC_F5,  KC_F6,  KC_F7,  KC_F8,  ES_LPRN, 		ES_EQL, KC_4, KC_5, KC_6, KC_PPLS, KC_PAST, \
		LLOCK, KC_F9,  KC_F10, KC_F11, KC_F12, ES_RPRN, 	KC_0, 	KC_1, KC_2, KC_3, ____,    ____, \
		                      ____, ____, ____, 		____,   KC_0, KC_PDOT),


      /*
      *  MOUSE
      * ┌────┬────┬────┬────┬────┬────┐     	┌────┬────┬────┬────┬────┬────┐
      * │F16 │___ │___ │F18 │___ │___ │     	│F17 │W▽  │ △  │W△  │M⏺1 │M⏺2 │
      * ├────┼────┼────┼────┼────┼────┤     	├────┼────┼────┼────┼────┼────┤
      * │___ │___ │___ │___ │___ │___ │     	│W◁  │ ◁  │ ▽  │ ▷  │W▷  │M⏹  │
      * ├────┼────┼────┼────┼────┼────┤     	├────┼────┼────┼────┼────┼────┤
      * │Lock│___ │ ⏸  │ ⏵  │ ⏮ │⏭   │     	│___ │F13 │F14 │F15 │M⏵1 │M⏵2 │
      * └────┴────┴────┴────┴────┴────┘     	└────┴────┴────┴────┴────┴────┘
      *                ┌────┐         			          ┌────┐
      *                │mute├────┐     			     ┌────┤Acc2│
      *                └────┤Vol-├────┐			┌────┤Acc1├────┘
      *                     └────┤Vol+│			│Acc0├────┘
      *                          └────┘ 		└────┘
      */
	[_MOUS] = LAYOUT_split_3x6_3( \
		KC_F16, ____, ____, KC_F18, ____, ____, 				KC_F17, 	KC_WH_D, KC_MS_U, KC_WH_U, QK_DYNAMIC_MACRO_RECORD_START_1, QK_DYNAMIC_MACRO_RECORD_START_2,  \
		____, ____, ____, ____, ____, ____, 				KC_WH_L,KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R, QK_DYNAMIC_MACRO_RECORD_STOP,  \
		LLOCK, ____, KC_MSTP, KC_MPLY, KC_MRWD, KC_MFFD, 	____, KC_F13, KC_F14, KC_F15, 	QK_DYNAMIC_MACRO_PLAY_1, QK_DYNAMIC_MACRO_PLAY_2,  \
							 KC_MUTE, KC_VOLD, KC_VOLU, 	KC_ACL0, KC_BTN2, KC_BTN1),


     /*
      *  SYMa
      * ┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
      * │___ │ "  │ <  │ >  │ '  │ \  │       │___ │___ │ _  │ &  │___ │___ │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │___ │ !  │ -  │ +  │ =  │ #  │       │ ñ  │ (  │ {  │ [  │ |  │___ │
      * ├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
      * │___ │ ^  │ /  │ *  │ $  │ ´  │       │ ?  │ )  │ }  │ ]  │SYMb│___ │
      * └────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
      *                ┌────┐                           ┌────┐
      *                │___ ├────┐                 ┌────┤___ │
      *                └────┤___ ├────┐       ┌────┤___ ├────┘
      *                     └────┤___ │       │___ ├────┘
      *                          └────┘       └────┘
      */
	[_SYMa] = LAYOUT_split_3x6_3( \
		____,	ES_DQUO,	ES_LABK,	ES_RABK, 	ES_QUOT,	LGUI(KC_GRV), 			____, 		____, 		ES_UNDS, 	ES_AMPR, 	____, 		____,  \
		____, 	ES_EXLM, 	ES_MINS, 	ES_PLUS, 	ES_EQL, 	ES_HASH, 				ES_NTIL, 	ES_LPRN, 	ES_LCBR, 	ES_LBRC, 	ES_PIPE,	____,  \
		____, 	ES_CIRC, 	ES_SLSH, 	ES_ASTR, 	ES_DLR, 	ES_ACUT, 				ES_QUES, 	ES_RPRN, 	ES_RCBR,	ES_RBRC, 	LSYMb, 		____,  \
											____, 			____, 		____, 			____, 		____, 		____),
     /*
      *  SYMb
      * ┌────┬────┬────┬────┬────┬────┐       	┌────┬────┬────┬────┬────┬────┐
      * │___ │ @  │___ │ `  │___ │../ │       	│___ │___ │ ¨  │___ │___ │___ │
      * ├────┼────┼────┼────┼────┼────┤       	├────┼────┼────┼────┼────┼────┤
      * │___ │ ¡  │ -> │___ │___ │___ │       	│ Ñ  │___ │___ │ ª  │ º  │___ │
      * ├────┼────┼────┼────┼────┼────┤       	├────┼────┼────┼────┼────┼────┤
      * │___ │___ │ %  │ ·  │ ${}│ ~  │       	│ ¿  │___ │___ │ ./ │___ │___ │
      * └────┴────┴────┴────┴────┴────┘       	└────┴────┴────┴────┴────┴────┘
      *                ┌────┐         			          ┌────┐
      *                │___ ├────┐         		     ┌────┤___ │
      *                └────┤___ ├────┐       	┌────┤___ ├────┘
      *                     └────┤___ │       	│___ ├────┘
      *                          └────┘       	└────┘
      */
	[_SYMb] = LAYOUT_split_3x6_3( \
		____,	ES_AT,		____,	ES_GRV, ____, 	DIRBACK, 					____,			____,	ES_DIAE, 	____, 			____, ____,  \
		____,	ES_IEXL,	ARROW, ____, 	____, 	____, 						RSFT(ES_NTIL), 	____, 	____, 		RSFT(KC_GRV), 	KC_GRV, ____,  \
		____, 	____, 		ES_PERC, ES_BULT, GROOVY_DOLLAR, ES_TILD, 		ES_IQUE, 		____, 	____, 		CURRDIR, 		____, ____,  \
										____, ____, ____, 						____, ____, ____),

    [_QWER] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,              KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                   KC_LGUI, KC_SPC, KC_BSPC,            KC_ENT, KC_SPC, KC_RALT
    )
};

bool user_keys(uint16_t keycode) {
	switch (keycode) {
		case ESCM:
		case KC_ESC:
			clear_oneshot_mods();
			caps_word_off();
			layer_lock_all_off();
            return true;
		case DIRBACK:
			//SEND_STRING("../");
            tap_code16(ES_DOT);
            tap_code16(ES_DOT);
            tap_code16(ES_SLSH);
            return false;
		case CURRDIR:
			//SEND_STRING("./");
			tap_code16(ES_DOT);
            tap_code16(ES_SLSH);
            return false;
        case ARROW:
			//SEND_STRING("->");
			tap_code16(ES_MINS);
            tap_code16(ES_RABK);
            return false;
        case GROOVY_DOLLAR:
			//SEND_STRING("${}" SS_TAP(X_LEFT));
			tap_code16(ES_DLR);
            tap_code16(ES_LCBR);
            tap_code16(ES_RCBR);
            tap_code16(KC_LEFT);            
            return false;
	}
	return true;
}

static bool shift = false;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	#ifdef CONSOLE_ENABLE
		uprintf("press: 0x%04X col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u %s\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count, shift?"shift":"");
	#endif

	if (!process_caps_word(keycode, record)) {
		return false;
	}
    if (!process_layer_lock(keycode, record, LLOCK)) {
        return false;
    }

	if (!chordmods_process(keycode, record)) {
		return false;
	}
	
	if (record->event.pressed && !user_keys(keycode)) {
		return false;
	}

	if (!is_caps_word_on()) {
		// toggle caps word on shift+space
		if (record->event.pressed && keycode == KC_RSFT) {
			shift = true;
		} else if (!record->event.pressed && keycode == KC_RSFT) {
			shift = false;
		} else if (shift && keycode == KC_SPC && !is_caps_word_on()) {
			if (record->event.pressed ) {
				caps_word_on();
			}
			shift = false;
			return false;
		}
	} else {
		shift = false;		
	}

	return true;
}


typedef enum {
	NONE,
	ONE,
	TWO,
	THREE,
	WAVE,
	CAPS
} blink_pattern_t;

static uint16_t blink_patterns[] = {0b0, 0b11, 0b1100011, 0b110001100011, 0b10100010101, 0b001011110101000};
static blink_pattern_t layer_blink_type = NONE;

void matrix_scan_user(void) {
	if (layer_blink_type == NONE) {
		backlight_level(8);
		//backlight_level(BACKLIGHT_LEVELS);
	} else if (layer_blink_type == WAVE) {
		uint32_t t = (timer_read32()/16)&0x3F;
		if (t > 31) t = (63 - t);		 
		backlight_level(t);
	} else {
		uint32_t t = (timer_read32()/40)&0xF;
		uint32_t level = blink_patterns[layer_blink_type]&(1<<t);
		backlight_level(level?0:BACKLIGHT_LEVELS);
	}
	
	layer_lock_task();
	chordmods_task(CHORDMOD_TERM);
}

void caps_word_set_user(bool active) {
    if (active) {
        // Do something when Caps Word activates.
		layer_blink_type = CAPS;
    } else {
        // Do something when Caps Word deactivates.
		layer_blink_type = NONE;
    }
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case ES_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}


layer_state_t layer_state_set_user (layer_state_t state) {
	/*if (is_caps_word_on()) return;
	
    layer_blink_timer = timer_read32();
    layer_blink_speed = 0;*/
    
    switch (get_highest_layer(state)) {
    case _NAV:
        layer_blink_type = ONE;
        break;
    case _NUM:
        layer_blink_type = TWO;
        break;
    case _MOUS:
        layer_blink_type = THREE;
        break;
    case _SYMa:
    case _SYMb:
        layer_blink_type = WAVE;
        break;
    case _COLEMAK:
    case _QWER:
        layer_blink_type = NONE;
        break;    
  }
  return state;
}
