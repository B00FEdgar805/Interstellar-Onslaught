//
//  DamageNumberSystem.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 9/19/26.
//

#ifndef DamageNumberSystem_hpp
#define DamageNumberSystem_hpp

#include "Components/DamageNumber.hpp"
#include "Camera.hpp"

class DamageNumberSystem
{
private:
    TTF_Font* FONT = nullptr;
    TTF_TextEngine* ENGINE = nullptr;
    std::vector <DamageNumber> NUM;
public:
    bool init(TTF_TextEngine* engine, TTF_Font* font);
    void spawn(int damage, const Vector2D& pos);
    void update(float deltaTime);
    void draw(const Camera2D& camera);
    void clear();
};


#endif /* DamageNumberSystem_hpp */
