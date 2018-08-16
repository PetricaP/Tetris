#ifndef VEC2_H
#define VEC2_H

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

#endif // VEC2_H
