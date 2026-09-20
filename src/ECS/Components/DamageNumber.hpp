//
//  DamageNumber.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 9/19/26.
//

#ifndef DamageNumber_hpp
#define DamageNumber_hpp

#include "Component.hpp"
#include "../../TextManager.hpp"

class DamageNumber final : public BaseComponent
{
public:
    TTF_Text* text = nullptr;
    float x = 0.0f;
    float y = 0.0f;
    float velocityY = -40.0f;
    float lifetime = 1.0f;
    float age = 0.0f;
    
    DamageNumber() = default;
    DamageNumber(TTF_Text* t, const Vector2D& pos)
    :
    text(t),
    x(pos.x),
    y(pos.y)
    {}
};

#endif /* DamageNumber_hpp */
