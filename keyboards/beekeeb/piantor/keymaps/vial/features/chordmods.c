
#include "chordmods.h"
#include "print.h"

typedef struct chordmod_t {
    int size;
    uint16_t* key;
    uint16_t* mod;
    uint16_t* pressed;
    uint32_t timer;
    uint32_t count;
    bool     issued;
} chordmod_t;

#define NR_CHORDMODS 2
uint16_t chorda[] = {KC_Z, KC_X, KC_C, KC_D, KC_V};
uint16_t modsa[] = {KC_LEFT_SHIFT, KC_LEFT_ALT, KC_LEFT_GUI, KC_LEFT_CTRL, KC_LEFT_CTRL};
uint16_t presseda[] = {0, 0, 0, 0, 0};

uint16_t chordb[] = {KC_DOT, KC_COMM, KC_H, KC_K};
uint16_t modsb[] = {KC_RIGHT_ALT, KC_RIGHT_GUI, KC_RIGHT_CTRL, KC_RIGHT_CTRL};
uint16_t pressedb[] = {0, 0, 0, 0, 0};

chordmod_t _chordmod_defs[NR_CHORDMODS] = {
  {5, chorda, modsa, presseda, 0, 0, false},
  {4, chordb, modsb, pressedb, 0, 0, false},
};

void update_mods(chordmod_t *chord) {
	if (!chord->issued) return;
	
	set_mods(0);
	uint8_t mods = 0;
	int c = 0;
	for (int i = 0; i < chord->size; i++) {
		if (chord->pressed[i]) {
#ifdef CONSOLE_ENABLE
	//uprintf("CHORD release: 0x%04X 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, keycode_layer, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
	uprintf("CHORD mod  %d\n", i);
#endif			
			mods |= MOD_BIT(chord->mod[i]);
			c++;
			if (c==chord->count-1) break; // !! Discard last mod
		}
	}
	
	register_mods(mods);
}

int is_chordmod_key(chordmod_t *chord, uint16_t keycode, keyrecord_t* record) {
    uint16_t keycode_layer = keymap_key_to_keycode(COMBO_ONLY_FROM_LAYER, record->event.key);
    for (int i = 0; i < chord->size; i++) {
        if (keycode_layer == chord->key[i]) {
            return i;
        }
    }
    return -1;
}

void pressed_key(chordmod_t *chord, int key_index, uint16_t keycode) {

    if (!chord->pressed[key_index]) {
        chord->count++;
        chord->pressed[key_index] = keycode;

        if (chord->issued) {
            // already on chord, just add the mod
            update_mods(chord);
        } else {
            chord->timer = timer_read32();
        }
    }
}

void cancel_chord(chordmod_t *chord) {
	if (chord->issued)
	    set_mods(0);

    chord->issued = false;
    chord->timer = 0;
    chord->count = 0;
    for (int i = 0; i < chord->size; i++) {
        if (chord->pressed[i]) {
            chord->pressed[i] = 0;
        }
    }
}

void released_key(chordmod_t *chord, int key_index, uint16_t keycode) {

    if (chord->pressed[key_index]) {
        if (!chord->issued) {
            unregister_code(chord->pressed[key_index]);
        }

        chord->count--;
        if (chord->count<0) chord->count = 0;
        chord->pressed[key_index] = 0;

        if (chord->issued) {
            // already on chord, remove the mod
            if (chord->count<2) {
                cancel_chord(chord);
            } else {
				update_mods(chord);
			}
        } else {
            chord->timer = 0;
        }
    }
}


bool chordmods_process(uint16_t keycode, keyrecord_t* record) {
    bool ret = true;

    if (record->event.pressed) {
        // pressed
        for (int comboid = 0; comboid < NR_CHORDMODS; comboid++) {
            chordmod_t* chord = &_chordmod_defs[comboid];
            int key_index = is_chordmod_key(chord, keycode, record);
			#ifdef CONSOLE_ENABLE
                uprintf("CHORD press: 0x%04X 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, key_index, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
            #endif

            if (key_index != -1) {
                // pressed combo key
                ret = false;
                pressed_key(chord, key_index, keycode);
                break;
            }
        }
    } else {
        // released
        for (int comboid = 0; comboid < NR_CHORDMODS; comboid++) {
            chordmod_t* chord = &_chordmod_defs[comboid];
            int key_index = is_chordmod_key(chord, keycode, record);
			#ifdef CONSOLE_ENABLE
                uprintf("CHORD release: 0x%04X 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, key_index, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
            #endif

            if (key_index != -1) {
                if (!chord->issued)
                    ret = false;
                released_key(chord, key_index, keycode);
                break;
            }
        }

    }
    return ret;
}

bool check_chord_issued(chordmod_t *chord) {
    chord->issued = false;
	if (chord->count == 2) {
		//last two key pressed must be consecutive
		for (int i = chord->size-1; i > 0; i--) {
			if (chord->pressed[i]) {
				if (chord->pressed[i-1])
					chord->issued = true;
				break;
			}
		}
	}
	return chord->issued;
}

void chordmods_task(void) {
    for (int comboid = 0; comboid < NR_CHORDMODS; comboid++) {
        chordmod_t* chord = &_chordmod_defs[comboid];
        if (!chord->issued && timer_elapsed32(chord->timer) > 50) {
            if (check_chord_issued(chord)) {
                update_mods(chord);
            } else {
                // not enough keys after the combo term
                // we cancel all and tap the last code
                if (chord->count == 1) {
                    for (int i; i < chord->size; i++) {
                        if (chord->pressed[i]) {
                            register_code(chord->pressed[i]);
                        }
                        break;
                    }
                }

                cancel_chord(chord);
            }
        }
    }
}
