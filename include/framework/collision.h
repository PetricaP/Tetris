#ifndef FRAMEWORK_COLLISION_H
#define FRAMEWORK_COLLISION_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    unsigned int w;
    unsigned int h;
} Box2D;

bool check_box_collision(const Box2D *box1, const Box2D *box2);

#endif // FRAMEWORK_COLLISION_H
