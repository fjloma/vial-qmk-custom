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
	_MODR,
	_MODL,
	_QWER
};

#define ____ KC_TRNS
#define ESCM LT(_MOUS,KC_ESC)
#define LNAV TL_LOWR
#define LNUM TL_UPPR
#define LSYMa MO(_SYMa)
#define LSYMb MO(_SYMb)


enum custom_keycode {
    B_DIRBACK = QK_KB_0,
    B_CURRDIR,
    B_ARROW,
    B_GROOVY_DOLLAR,
    LLOCK
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*  0 COLEMAK DH
      * ┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
      * │EscM│ Q  │ W  │ F	│ P  │ B  │       │ J  │ L  │ U  │ Y  │Bsp │Del │
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
      * │Lock│___ │ ⏸  │ ⏵  │ ⏮ │⏭	  │     	│___ │F13 │F14 │F15 │M⏵1 │M⏵2 │
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
							 KC_MUTE, KC_VOLD, KC_VOLU, 	KC_ACL0, KC_ACL2, KC_ACL1),


     /*
      *  SYMa
      * ┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
      * │___ │ "  │ <  │ >	│ '  │ \  │       │___ │___ │ _  │ &  │___ │___ │
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
      * │___ │ @  │___ │ `	│___ │../ │       	│___ │___ │ ¨  │___ │___ │___ │
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
		____,	ES_AT,		____,	ES_GRV, ____, 	B_DIRBACK, 					____,			____,	ES_DIAE, 	____, 			____, ____,  \
		____,	ES_IEXL,	B_ARROW, ____, 	____, 	____, 						RSFT(ES_NTIL), 	____, 	____, 		RSFT(KC_GRV), 	KC_GRV, ____,  \
		____, 	____, 		ES_PERC, ES_BULT, B_GROOVY_DOLLAR, ES_TILD, 		ES_IQUE, 		____, 	____, 		B_CURRDIR, 		____, ____,  \
										____, ____, ____, 						____, ____, ____),

      /*
	  *  Mod
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │___│___│___│___│___│___│       │___│___│___│___│___│___│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │___│___│___│___│___│___│       │___│___│___│___│___│___│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │___│___│___│___│___│___│       │Ctr│Gui│Gui│Alt│Sft│___│
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      */
	[_MODR] = LAYOUT_split_3x6_3( \
		____, ____, ____, ____, ____, ____, 	____, ____, ____, ____, ____, ____, \
		____, ____, ____, ____, ____, ____, 	____, ____, ____, ____, ____, ____, \
		____, ____, ____, ____, ____, ____, 	KC_RCTL, KC_RGUI, KC_RGUI, KC_RALT, KC_RSFT, ____,  \
					      ____, ____, ____, 	____, ____, ____),

      /*
	  *  Mod
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │___│___│___│___│___│___│       │___│___│___│___│___│___│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │___│___│___│___│___│___│       │___│___│___│___│___│___│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │___│Sft│Alt│Gui│Gui│Ctr│       │___│___│___│___│___│___│
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      */
	[_MODL] = LAYOUT_split_3x6_3( \
		____, ____, ____, ____, ____, ____, 	____, ____, ____, ____, ____, ____, \
		____, ____, ____, ____, ____, ____, 	____, ____, ____, ____, ____, ____, \
		____, KC_LSFT, KC_LALT, KC_LGUI, KC_LGUI, KC_LCTL, 	____, ____, ____, ____, ____, ____,  \
					      ____, ____, ____, 	____, ____, ____),

    [_QWER] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,              KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                   KC_LGUI, KC_SPC, KC_BSPC,            KC_ENT, KC_SPC, KC_RALT
    )
};


static bool shift = false;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

	if (!process_caps_word(keycode, record)) {
		return false;
	}
    if (!process_layer_lock(keycode, record, LLOCK)) {
        return false;
    }

	if (!chordmods_process(keycode, record)) {
		return false;
	}

	switch (keycode) {
		/*case KC_ESC:
                clear_oneshot_mods();
                caps_word_off();
                layer_lock_all_off();
                break;
        */
		case B_DIRBACK:
            if (record->event.pressed) {
                SEND_STRING("../");
            }
            return false;
		case B_CURRDIR:
            if (record->event.pressed) {
                SEND_STRING("./");
            }
            return false;
        case B_ARROW:
            if (record->event.pressed) {
                SEND_STRING("->");
            }
            return false;
        case B_GROOVY_DOLLAR:
            if (record->event.pressed) {
                SEND_STRING("${}" SS_TAP(X_LEFT));
            }
            return false;
	}

    // toggle caps word on shift+space

    if (keycode == KC_RSFT) {
        shift = record->event.pressed;
        backlight_toggle();
        wait_ms(100);
        backlight_toggle();

    } else if (record->event.pressed && keycode == KC_SPC) {

        if (shift) {
            caps_word_toggle();
            return false;
        }
    }



	return true;
}

static uint32_t layer_blink_timer = 0;
static uint32_t layer_blink_speed = 500;
static bool left_blink = false;
static bool right_blink = false;

void matrix_scan_user(void) {
  layer_lock_task();
  chordmods_task();

  if (is_keyboard_master() && left_blink && layer_blink_timer && timer_elapsed32(layer_blink_timer) > layer_blink_speed){
    layer_blink_timer = timer_read32();
    backlight_toggle();
  } else if (!is_keyboard_master() && right_blink && layer_blink_timer && timer_elapsed32(layer_blink_timer) > layer_blink_speed){
    layer_blink_timer = timer_read32();
    backlight_toggle();
  }
}


void layer_lock_set_user(layer_state_t locked_layers) {
   if (locked_layers) {
        right_blink = true;
        left_blink = true;
        layer_blink_timer = timer_read32();
   } else {
       layer_blink_timer = 0;
   }
}


layer_state_t layer_state_set_user (layer_state_t state) {
    right_blink = false;
    left_blink = false;
    layer_blink_timer = 0;

    switch (get_highest_layer(state)) {
    case _NAV:
        right_blink = true;
        left_blink = false;
        layer_blink_timer = timer_read32();
        break;
    case _NUM:
        right_blink = false;
        left_blink = true;
        layer_blink_timer = timer_read32();
        break;
    case _MOUS:
        right_blink = true;
        left_blink = true;
        layer_blink_timer = timer_read32();
        break;
  }
  return state;
}


/*
enum combo_events {
  ZX_SHF,
  XC_ALT,
  CD_GUI,
  DV_CTRL,
  Hc_GUI,//h,
  cp_ALT, //,.
  KH_CTRL
};

const uint16_t PROGMEM lsft_combo[] = {KC_Z, KC_X, COMBO_END};
const uint16_t PROGMEM lalt_combo[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM lgui_combo[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM lctl_combo[] = {KC_D, KC_V, COMBO_END};

const uint16_t PROGMEM rgui_combo[] = {KC_H, KC_COMM, COMBO_END};
const uint16_t PROGMEM ralt_combo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM rctl_combo[] = {KC_K, KC_H, COMBO_END};


combo_t key_combos[] = {
  [ZX_SHF] = COMBO_ACTION(lsft_combo),
  [XC_ALT] = COMBO_ACTION(lalt_combo),
  [CD_GUI] = COMBO_ACTION(lgui_combo),
  [DV_CTRL] = COMBO_ACTION(lctl_combo),
  [Hc_GUI] = COMBO_ACTION(rgui_combo),
  [cp_ALT] = COMBO_ACTION(ralt_combo),
  [KH_CTRL] = COMBO_ACTION(rctl_combo)
};

void process_combo_event(uint16_t combo_index, bool pressed){
  switch(combo_index) {

    case cp_ALT:
      if (pressed) {
	      register_mods(MOD_BIT(KC_RIGHT_ALT));
	      layer_on(_MODR);
      } else {
		  unregister_mods(MOD_BIT(KC_RIGHT_ALT));
		  layer_off(_MODR);
	  }
      break;

    case Hc_GUI:
      if (pressed) {
	      register_mods(MOD_BIT(KC_RIGHT_GUI));
	      layer_on(_MODR);
      } else {
		  unregister_mods(MOD_BIT(KC_RIGHT_GUI));
		  layer_off(_MODR);
	  }
      break;

    case KH_CTRL:
      if (pressed) {
	      register_mods(MOD_BIT(KC_RIGHT_CTRL));
	      layer_on(_MODR);
      } else {
		  unregister_mods(MOD_BIT(KC_RIGHT_CTRL));
		  layer_off(_MODR);
	  }
      break;



    case ZX_SHF:
      if (pressed) {
	      register_mods(MOD_BIT(KC_LEFT_SHIFT));
	      layer_on(_MODL);
      } else {
		  unregister_mods(MOD_BIT(KC_LEFT_SHIFT));
	      layer_off(_MODL);
	  }
      break;

    case XC_ALT:
      if (pressed) {
	      register_mods(MOD_BIT(KC_LEFT_ALT));
	      layer_on(_MODL);
      } else {
		  unregister_mods(MOD_BIT(KC_LEFT_ALT));
	      layer_off(_MODL);
	  }
      break;

    case CD_GUI:
      if (pressed) {
	      register_mods(MOD_BIT(KC_LEFT_GUI));
	      layer_on(_MODL);
      } else {
		  unregister_mods(MOD_BIT(KC_LEFT_GUI));
	      layer_off(_MODL);
	  }
      break;

    case DV_CTRL:
      if (pressed) {
	      register_mods(MOD_BIT(KC_LEFT_CTRL));
	      layer_on(_MODL);
      } else {
		  unregister_mods(MOD_BIT(KC_LEFT_CTRL));
	      layer_off(_MODL);
	  }
      break;
  }
}
*/
