#ifndef FRAMEWORK_INPUT_MANAGER_H
#define FRAMEWORK_INPUT_MANAGER_H

#include <stdbool.h>

#define NUM_KEYS 256

void press_key(unsigned int keyID);
void release_key(unsigned int keyID);
bool is_key_pressed(unsigned int keyID);

#endif // FRAMEWORK_INPUT_MANAGER_H
