#pragma once
#include <iostream>

class Timer
{
public:
  Timer();
  void Update(float dt);
  void Restart();
  float Get();

private:
  float time;
};