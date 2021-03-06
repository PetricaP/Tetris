#include "vec2.h"

vec2f add_vec2f(vec2f first, vec2f second) {
    vec2f result = {first.x + second.x, first.y + second.y};
    return result;
}

vec2i add_vec2i(vec2i first, vec2i second) {
    vec2i result = {first.x + second.x, first.y + second.y};
    return result;
}

vec2f mul_vec2f_f(vec2f vec, float scale) {
    vec2f newVec = {vec.x * scale, vec.y * scale};
    return newVec;
}
