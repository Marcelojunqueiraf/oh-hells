#include "InputManager.hpp"

InputManager::InputManager() : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0)
{
  for (int i = 0; i < 6; i++)
  {
    mouseState[i] = false;
    mouseUpdate[i] = 0;
  }
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
  SDL_Event event;

  SDL_GetMouseState(&mouseX, &mouseY);

  quitRequested = false;

  updateCounter++;

  for (int i = 0; i < 6; i++)
  {
    if (mouseUpdate[i] == updateCounter)
    {
      mouseUpdate[i] = 0;
    }
  }

  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      quitRequested = true;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      mouseState[event.button.button] = true;
      mouseUpdate[event.button.button] = updateCounter;
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
      mouseState[event.button.button] = false;
      mouseUpdate[event.button.button] = updateCounter;
    }
    else if (event.type == SDL_KEYDOWN)
    {
      keyState[event.key.keysym.sym] = true;
      keyUpdate[event.key.keysym.sym] = updateCounter;
    }
    else if (event.type == SDL_KEYUP)
    {
      keyState[event.key.keysym.sym] = false;
      keyUpdate[event.key.keysym.sym] = updateCounter;
    }
  }
}

bool InputManager::KeyPress(int key)
{
  return (keyUpdate[key] == updateCounter && keyState[key] == true);
}

bool InputManager::KeyRelease(int key)
{
  return (keyUpdate[key] == updateCounter && keyState[key] == false);
}

bool InputManager::IsKeyDown(int key)
{
  return keyState[key];
}

bool InputManager::MousePress(int button)
{
  return (mouseUpdate[button] == updateCounter && mouseState[button] == true);
}

bool InputManager::MouseRelease(int button)
{
  return (mouseUpdate[button] == updateCounter && mouseState[button] == false);
}

bool InputManager::IsMouseDown(int button)
{
  return mouseState[button];
}

int InputManager::GetMouseX()
{
  Camera &camera = Camera::GetInstance();
  return mouseX + camera.pos.x;
}

int InputManager::GetMouseY()
{
  Camera &camera = Camera::GetInstance();
  return mouseY + camera.pos.y;
}

bool InputManager::QuitRequested()
{
  return quitRequested;
}

InputManager &InputManager::GetInstance()
{
  static InputManager instance;
  return instance;
}
