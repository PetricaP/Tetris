#ifndef FRAMEWORK_VEC2_H
#define FRAMEWORK_VEC2_H

typedef struct {
    float x;
    float y;
} vec2f;

typedef struct {
    int x;
    int y;
} vec2i;

vec2f add_vec2f(vec2f first, vec2f second);
vec2i add_vec2i(vec2i first, vec2i second);
vec2f mul_vec2f_f(vec2f vec, float scale);

#endif // FRAMEWORK_VEC2_H
