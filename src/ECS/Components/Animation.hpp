#ifndef Animation_hpp
#define Animation_hpp

#include "Component.hpp"

class Animation final : public BaseComponent
{
public:
    int speed = 0;
    int frames = 0;
    float delta = 0;
    Animation() = default;
    Animation(float delta, int frames, int speed)
    :   delta(delta),
        frames(frames),
        speed(speed)
    {}
    
};


#endif /* Animation_hpp */
