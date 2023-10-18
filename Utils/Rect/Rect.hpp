#pragma once

#include "../Vec2/Vec2.hpp"

class Rect
{
public:
  float x, y, w, h;
  Rect(float x, float y, float w, float h);
  Rect();
  bool Contains(float x, float y);
  void SetCenter(float x, float y);
  Vec2 GetCenter();
};