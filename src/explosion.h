#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <sp2/scene/particleEmitter.h>

class Explosion : public sp::ParticleEmitter
{
public:
    Explosion(sp::Vector2d position, float size, sp::Vector2d velocity=sp::Vector2d(0, 0));
};

#endif//EXPLOSION_H
