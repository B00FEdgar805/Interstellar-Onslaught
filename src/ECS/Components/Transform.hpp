#ifndef Transform_hpp
#define Transform_hpp

#include "Component.hpp"
//#include "../../Math.hpp"

class Transform final : public BaseComponent 
{
public:
    Vector2D position;
    
    Transform() = default;

    //Transform(float x, float y) // Sets x and y for enetity
      //  : x(x), y(y) {}
    Transform(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    
    Transform(const Vector2D& v)
    {
        position = v;
    }
};

#endif /* Transform.hpp */
