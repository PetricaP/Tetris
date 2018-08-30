#include "collision.h"
#include <stdio.h>

bool check_box_collision(const Box2D *box1, const Box2D *box2) {
    if (box2->x < box1->x + (int)box1->w && box2->x + (int)box2->w > box1->x &&
        box2->y < box1->y + (int)box1->h && box2->y + (int)box2->h > box1->y) {
        return true;
    }
    return false;
}
