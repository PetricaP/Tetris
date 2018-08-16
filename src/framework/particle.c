#include "particle.h"
#include "circle.h"
#include <stdlib.h>

#define MAX_LIFETIME 1000.0f

const vec2f GRAVITY = { 0.0f, 0.05f };

Particle create_particle(vec2f position, int radius, int lifeTime, vec2f velocity, vec4u color) {
    Particle particle;
    particle.position = position;
    particle.velocity = velocity;
    particle.lifeTime = lifeTime;
    particle.currentLifeTime = lifeTime;
    particle.color = color;
    particle.radius = radius;
    return particle;
}

void update_particle(Particle *particle) {
    particle->position = add_vec2f(particle->position, particle->velocity);
    particle->velocity = add_vec2f(particle->velocity, GRAVITY);
    if (particle->lifeTime != 0) {
        --particle->currentLifeTime;
        particle->color.a = (float) particle->currentLifeTime / particle->lifeTime * 255;
    }
}

void draw_particle(Particle *particle) {
    vec2i position = { particle->position.x, particle->position.y };
    draw_circle(position, particle->radius, particle->color);
}

