#pragma once

#include <cmath>
class Vec2
{
public:
  float x, y;
  Vec2();
  Vec2(float x, float y);
  Vec2 operator+(Vec2 v);
  Vec2 operator-(Vec2 v);
  Vec2 operator*(float n);
  Vec2 operator/(float n);
  float magnitude();
  Vec2 normalize();
  Vec2 rotate(float angle);
  float getAngle();
};