#include "circle.h"
#include "particle.h"
#include <stdlib.h>

#define MAX_LIFETIME 1000.0f

const vec2f GRAVITY = {0.0f, 0.0f};

Particle create_particle(vec2f position, int radius, int lifeTime,
                         vec2f velocity, vec4u color) {
    Particle particle;
    particle.position = position;
    particle.velocity = velocity;
    particle.lifeTime = lifeTime;
    particle.currentLifeTime = lifeTime;
    particle.color = color;
    particle.radius = radius;
    return particle;
}

void update_particle(Particle *particle, float deltaTime) {
    particle->velocity =
        add_vec2f(particle->velocity, mul_vec2f_f(GRAVITY, deltaTime * 0.03));
    particle->position = 
        add_vec2f(particle->position, mul_vec2f_f(particle->velocity, deltaTime * 0.3));
    if (particle->lifeTime != 0) {
        particle->currentLifeTime -= deltaTime * 0.07;
        if (particle->currentLifeTime > 0) {
            particle->color.a =
                (float)particle->currentLifeTime / particle->lifeTime * 255;
        }
    }
}

void draw_particle(Particle *particle) {
    vec2i position = {particle->position.x, particle->position.y};
    draw_circle(position, particle->radius, particle->color);
}
