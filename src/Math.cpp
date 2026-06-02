#include "Math.hpp"

Vector2D::Vector2D()
{
    this -> x = 0.0f;
    this -> y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
    this -> x = x;
    this -> y = y;
}

Vector2D& Vector2D::add(const Vector2D& v)
{
    this -> x += v.x;
    this -> y += v.y;
    
    return *this;
}

Vector2D& Vector2D::subtract(const Vector2D& v)
{
    this -> x -= v.x;
    this -> y -= v.y;
    
    return *this;
}

Vector2D& Vector2D::mutiplie(const Vector2D& v)
{
    this -> x *= v.x;
    this -> y *= v.y;
    
    return *this;
}

Vector2D& Vector2D::divide(const Vector2D& v)
{
    this -> x /= v.x;
    this -> y /= v.y;
    
    return *this;
}

Vector2D& Vector2D::scale(float scaler)
{
    this -> x *= scaler;
    this -> y *= scaler;
    
    return *this;
}

Vector2D& Vector2D::normalize()
{
    float l = std::sqrt((this -> x * this -> x) + (this -> y * this -> y));
   // v.x /= l;
    //v.y /= l;
    this -> x /= l;
    this -> y /= l;
    
    return *this;
}


Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
    return v1.add(v2);
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
    return v1.subtract(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
    return v1.mutiplie(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
    return v1.divide(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
    return this -> add(v);
}

Vector2D& Vector2D::operator-=(const Vector2D& v)
{
    return this -> subtract(v);
}

Vector2D& Vector2D::operator*=(const Vector2D& v)
{
    return this -> mutiplie(v);
}

Vector2D& Vector2D::operator/=(const Vector2D& v)
{
    return this -> divide(v);
}

std::ostream& operator << (std::ostream& stream ,const Vector2D& v)
{
    stream << "Vector2D: (" << v.x << "," << v.y << ")" ;
    return stream;
}
