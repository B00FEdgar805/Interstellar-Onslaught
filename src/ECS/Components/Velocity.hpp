#ifndef Velocity_hpp
#define Velocity_hpp

#include "Component.hpp"

class Velocity final : public BaseComponent
{
public:
    Vector2D value;
    
    Velocity() = default;
    
    Velocity(float x, float y)
    {
        value.x = x;
        value.y = y;
    }
    
    Velocity(const Vector2D& v)
    {
        value = v;
    }
};
#endif /* Velocity_hpp */
