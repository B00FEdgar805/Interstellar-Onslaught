#ifndef Transform_hpp
#define Transform_hpp

#include "Component.hpp"

class Transform final : public BaseComponent 
{
public:
    float x = 0.0f;
    float y = 0.0f;
    
    Transform() = default;

    Transform(float x, float y) // Sets x and y for enetity
        : x(x), y(y) {}
};

#endif /* Transform.hpp */
