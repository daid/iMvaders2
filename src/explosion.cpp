#include "explosion.h"
#include "spacescene.h"
#include <sp2/random.h>

Explosion::Explosion(sp::Vector2d position, float size, sp::Vector2d velocity)
: sp::ParticleEmitter(space_scene->getRoot())
{
    auto_destroy = true;
    setPosition(position);
    
    render_data.type = sp::RenderData::Type::Additive;
    
    Parameters parameters;
    //Main flash
    for(int n=0; n<10; n++)
    {
        parameters.position = sp::Vector3f(0, 0, 0);
        parameters.velocity = (sp::Vector3f(sp::random(-1, 1), sp::random(-1, 1), 0.0)) * sp::random(1.5 * size, 2.5 * size) + sp::Vector3f(velocity.x * 0.7, velocity.y * 0.7, 0).normalized();
        //parameters.acceleration = -parameters.velocity * 0.5f;
        //parameters.start_size = size * 0.7;
        //parameters.end_size = size * 0.3;
        //parameters.start_color = sp::Color(1, 0.5, 0.2);
        //parameters.end_color = sp::Color(0, 0, 0);
        parameters.lifetime = 0.25;
        emit(parameters);
    }

    //Outwards particles
    for(int n=0; n<30; n++)
    {
        parameters.position = sp::Vector3f(sp::random(-1, 1), sp::random(-1, 1), 0.0).normalized() * sp::random(0.3 * size, 0.5 * size);
        parameters.velocity = parameters.position * 7.0f + sp::Vector3f(velocity.x, velocity.y, 0);
        //parameters.acceleration = -parameters.velocity;
        //parameters.start_size = size * 0.3;
        //parameters.end_size = 0.0;
        //parameters.start_color = sp::Color(1, 0.2, 0.1);
        //parameters.end_color = sp::Color(0, 0, 0);
        parameters.lifetime = 0.4;
        emit(parameters);
    }
}
