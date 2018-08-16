#ifndef FRAMEWORK_PARTICLE_H
#define FRAMEWORK_PARTICLE_H

#include "vec2.h"
#include "vec4.h"

typedef struct {
    int lifeTime;
    int radius;
    int currentLifeTime;
    vec2f position;
    vec2f velocity;
    vec4u color;
} Particle;

Particle create_particle(vec2f position, int radius, int lifeTime, vec2f velocity, vec4u color);
void draw_particle(Particle *particle);
void update_particle(Particle *particle);

#endif // FRAMEWORK_PARTICLE_H
