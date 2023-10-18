#include "Rect.hpp"
#include <iostream>

Rect::Rect(float x, float y, float w, float h)
{
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
}

Rect::Rect()
{
  this->x = 0;
  this->y = 0;
  this->w = 0;
  this->h = 0;
}

bool Rect::Contains(float x, float y)
{
  return (x >= this->x && x <= this->x + this->w && y >= this->y && y <= this->y + this->h);
}

void Rect::SetCenter(float x, float y)
{
  this->x = x - this->w / 2;
  this->y = y - this->h / 2;
}

Vec2 Rect::GetCenter()
{
  return Vec2(this->x + this->w / 2, this->y + this->h / 2);
}