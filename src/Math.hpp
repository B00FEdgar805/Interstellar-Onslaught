//
//  Math.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/21/26.
//

#ifndef Math_hpp
#define Math_hpp

#include <iostream>

struct Vector2D
{
    float x = 0.0f;
    float y = 0.0f;
    
    Vector2D();
    Vector2D(float x, float y);
    Vector2D& add(const Vector2D& v);
    Vector2D& subtract(const Vector2D& v);
    Vector2D& mutiplie(const Vector2D& v);
    Vector2D& divide(const Vector2D& v);
    Vector2D& scale(float scaler);
    
    friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& v);
    Vector2D& operator-=(const Vector2D& v);
    Vector2D& operator*=(const Vector2D& v);
    Vector2D& operator/=(const Vector2D& v);
    
    friend std::ostream& operator << (std::ostream& stream ,const Vector2D& v);
};





#endif /* Math_hpp */
