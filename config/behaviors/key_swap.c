// #include <zmk/behavior.h>
// #include <zmk/behavior_queue.h>
// #include <zmk/event_manager.h>
// #include <zmk/events/position_state_changed.h>
// #include <zmk/keymap.h>
// #include <zmk/keys.h>
// #include <zmk/matrix.h>
// #include <zmk/behavior_binding.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define max_pairs 5

static bool any_active_pairs = false;

//pair of 2 keys and an active/inactive bool
struct keyPair {
    uint8_t key_1;
    uint8_t key_2;
    bool pair_active;
};

//create an array of 5 keyPairs to store at any one time
//create a pointer that will be used to point to the most recently typed keyPair in pairs[]
static struct keyPair pairs[max_pairs];
static struct keyPair *current_pair_pointer = NULL;

//spits out a new active pair
static struct keyPair generatePair(uint8_t pressed_key_1, uint8_t pressed_key_2) {
    struct keyPair newPair = {pressed_key_1, pressed_key_2, true};
    any_active_pairs = true;
    return newPair;
};

static bool checkForActivePairs(void) {
    for (int i=0; i < max_pairs; i++) {
        if (pairs[i].key_1 == 0) {
            continue;
        }
        if (pairs[i].pair_active == true) {
            any_active_pairs = true;
            return true;
        }
    }
    any_active_pairs = false;
    return false;
}
//returns a pointer to the next empty slot of pairs[] or slot 0 if none are empty
static struct keyPair *findSlot(void){
    for (int i = 0; i < max_pairs; i++) {
        if (pairs[i].key_1 == 0) {
            return &pairs[i];
        }
    }
    return &pairs[0];
}

//takes in 1 key that will be what the user types
//if no pairs are active, return the same key
//otherwise, check if the first pair in pairs[] is active
//if not, check the next pair
//if so, check if the typed key is in that pair
//if not, check the next pair
//if so, return the other key in the pair
//if it's not in pairs[] at all, return the same key
static uint8_t performKeySwap(uint8_t key_to_swap) {
    if (any_active_pairs == false) {
        return key_to_swap;
    }
    for (int i = 0; i < max_pairs; i++) {
        if (pairs[i].pair_active == false) {
            continue;
        }
        if (pairs[i].key_1 == key_to_swap) {
            current_pair_pointer = &pairs[i];
            return pairs[i].key_2;
        }
        else if (pairs[i].key_2 == key_to_swap) {
            current_pair_pointer = &pairs[i];
            return pairs[i].key_1;
        }
    }
    return key_to_swap;
}


//if: keylk + 2 different keys are held
//then: takes in 2 keys that will be from user input
//find an open slot
//point to it with most_recent_pair_pointer
//generate a pair with the keys
//fill the adress pointed to with the new pair
//result: new pair is in pairs[], can be referenced by recent pair pointer
static void autoCreatePair(uint8_t pressed_key_1, uint8_t pressed_key_2) {
    current_pair_pointer = findSlot();
    *current_pair_pointer = generatePair(pressed_key_1, pressed_key_2);
}

//if there is a most recently used or currently selected pair, toggle it
static void toggleCurrentPair(void) {
    if (verifyCurrentPointer() == false) {
        return;
    }
    current_pair_pointer->pair_active = !current_pair_pointer->pair_active;
    checkForActivePairs();
}

//if: keylk is double tapped
//then: goes through every pair in [pairs]
//if a pair is empty, skip it
//otherwise, turn it on if active_swap is false, or turn it off if active_swap is true
//result: everything on or everything off unless it's empty
static void toggleAllPairs(void) {
    for (int i = 0; i < max_pairs; i++) {
        if (pairs[i].key_1 == 0) {
            continue;
        }
        pairs[i].pair_active = !any_active_pairs;
    }
    checkForActivePairs();
}

//if: keylk and a number 1 - maxslots is typed
//then set the current pair pointer to the address of that number in pairs but in the right base
//ONLY RUN THIS IF KEY1 == KEY2
static void selectPairByNumber(uint8_t slot) {
    if (slot > 0 && slot <= max_pairs) {
        current_pair_pointer = &pairs[slot-1];
    }
}

static bool selectPairByKey(uint8_t key) {
    for (int i = 0; i < max_pairs; i++) {
        if (pairs[i].key_1 == 0) {
            continue;
        }
        if (key == pairs[i].key_1 || key == pairs[i].key_2) {
            current_pair_pointer = &pairs[i];
            return true;
        }
    }
    return false;
}

static void replaceSpecificPair(uint8_t original_key, uint8_t new_key_1, uint8_t new_key_2) {
    if (selectPairByKey(original_key) == true) {
        *current_pair_pointer = generatePair(new_key_1, new_key_2);
    }

}

static bool verifyCurrentPointer(void) {
    if (current_pair_pointer == NULL) {
        current_pair_pointer = &pairs[0];
        return false;
    }
    else {
        return true;
    }
}



int main(void) {
    memset(pairs, 0, sizeof(pairs));
    verifyCurrentPointer();
    return 0;
}

// hold keylk and 2 other keys: sets pair
// double tap keylk: toggle all pairs
// tap keylk: toggle most recent pair
// hold keylk, 1 old key and 2 new keys: replace 

//if you want to set a new pair in any way: hold + 2 keys
//if you want to replace the current slot: 