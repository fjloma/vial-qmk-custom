
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

int is_chordmod_key(chordmod_t *chord, uint16_t keycode, keyrecord_t* record) {
    uint16_t keycode_layer = keycode;//keymap_key_to_keycode(COMBO_ONLY_FROM_LAYER, record->event.key);
    #ifdef CONSOLE_ENABLE
        uprintf("kc: 0x%04X 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, keycode_layer, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    #endif
    for (int i; i < chord->size; i++) {
        if (keycode_layer == chord->key[i]) {
            #ifdef CONSOLE_ENABLE
                uprintf("CHORD: 0x%04X 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, keycode_layer, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
            #endif
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
            register_mods(chord->mod[key_index]);
        } else {
            chord->timer = timer_read32();
        }
    }
}

void cancel_chord(chordmod_t *chord) {
    // released but not yet issued, tap it
    if (!chord->issued && chord->count == 1) {
        for (int i; i < chord->size; i++) {
            if (chord->pressed[i]) {
                tap_code16(chord->pressed[i]);
                break;
            }
        }
    }

    chord->issued = false;
    chord->timer = 0;
    chord->count = 0;
    uint8_t mods = 0;
    for (int i = 0; i < chord->size; i++) {
        if (chord->pressed[i]) {
            mods |= MOD_BIT(chord->mod[i]);
            chord->pressed[i] = 0;
        }
    }
    unregister_mods(mods);

}

void released_key(chordmod_t *chord, int key_index, uint16_t keycode) {

    if (chord->pressed[key_index]) {
        if (chord->issued) {
            // already on chord, remove the mod
            if (chord->pressed[key_index]) unregister_mods(chord->mod[key_index]);
        }

        chord->count--;
        if (chord->count<0) chord->count = 0;
        chord->pressed[key_index] = 0;

        if (chord->issued) {
            // already on chord, remove the mod
            if (chord->count<2) {
                cancel_chord(chord);
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
            if (key_index != -1) {
                tap_code16(KC_FIND);
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
            if (key_index != -1) {
                tap_code16(KC_CUT);
                if (!chord->issued)
                    ret = false;
                released_key(chord, key_index, keycode);
                break;
            }
        }

    }
    return ret;
}

void chordmods_task(void) {
    for (int comboid = 0; comboid < NR_CHORDMODS; comboid++) {
        chordmod_t* chord = &_chordmod_defs[comboid];
        if (!chord->issued && timer_elapsed32(chord->timer) > 5000) {
            if (chord->count>=2) {
                chord->issued = true;
                uint8_t mods = 0;
                int c = 0;
                for (int i = 0; i < chord->size; i++) {
                    if (chord->pressed[i]) {
                        mods |= MOD_BIT(chord->pressed[i]);
                        c++;
                        //if (c==chord->size) break; // !! Discard last mod
                    }
                }
                register_mods(mods);
            } else {
                // not enough keys after the combo term
                // we cancel all and tap the last code
                cancel_chord(chord);
            }
        }
    }
}
