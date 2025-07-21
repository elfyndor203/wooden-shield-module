#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/keys.h>
#include <zmk/matrix.h>
#include <zmk/behavior_binding.h>

// #define defined_swaps 1

#define active_swap false
#define max_pairs 5

static struct keyPair {
    uint8_t key_1;
    uint8_t key_2;
    bool pair_active;
} pairs[];

static logPair(uint8_t pressed_key1, uint8_t pressed_key2, struct key_pair assigned_pair) {
    pressed_key1 = assigned_pair.key_1
    pressed_key2 = assigned_pair.key_2
};

static findSlot(struct keyPair newPair){
    for (int i = 0; i < max_pairs; i++) {
        if pairs[i].key_1 
    }
}

static swapPair(uint8_t key_to_swap) {
    for (int i = 0; i < max_pairs; i++) {
        if (pairs[i].active == false) continue;
        if (pairs[i].key_1 == key_to_swap) return pairs[i].key_2
        else if (pairs[i].key_2 == key_to_swap) return pairs[i].key_1
    }
    return key_to_swap;
}
