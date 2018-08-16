#include "input_manager.h"

static bool m_key_map[NUM_KEYS] = {false};

void press_key(unsigned int keyID) { m_key_map[keyID % NUM_KEYS] = true; }

void release_key(unsigned int keyID) { m_key_map[keyID % NUM_KEYS] = false; }

bool is_key_pressed(unsigned int keyID) { return m_key_map[keyID % NUM_KEYS]; }
