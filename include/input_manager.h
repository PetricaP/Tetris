#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <stdbool.h>

#define NUM_KEYS 256

void press_key(unsigned int keyID);
void release_key(unsigned int keyID);
bool is_key_pressed(unsigned int keyID);

#endif
