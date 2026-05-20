//
//  Position.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/19/26.
//

#ifndef Transform_hpp
#define Transform_hpp

#include "Component.hpp"

class Transform final : public BaseComponent 
{
public:
    float x = 0.0f;
    float y = 0.0f;

public:
    Transform() = default;

    Transform(float x, float y)
        : x(x), y(y) {
    }
};

#endif /* Transform.hpp */
