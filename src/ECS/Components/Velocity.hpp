#ifndef Velocity_hpp
#define Velocity_hpp

#include "Component.hpp"

class Velocity final : public BaseComponent
{
public:
    float x = 0.0f;
    float y = 0.0f;
    
    Velocity() = default;

    Velocity(float x, float y)  // Sets x and y velocity for entity
        : x(x), y(y) {
    }
};
#endif /* Velocity_hpp */
