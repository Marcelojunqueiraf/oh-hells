#include "State.hpp"
#include "../Componentes/Player/Player.hpp"
#include "../Utils/Collision/Collision.cpp"

State::State()
{
  quitRequested = false;
  objectArray = std::vector<std::shared_ptr<GameObject>>();
}

State::~State()
{
}

void State::LoadAssets()
{
  GameObject *go = new GameObject();
  std::weak_ptr<GameObject> goPtr = this->AddObject(go);
  Sprite *bg = new Sprite("Assets/fundo.png", goPtr);
  go->AddComponent(bg);
  go->AddComponent(new CameraFollower(goPtr));

  // Adicionando mapa
  bg = new Sprite("Assets/mapa_portal_preguica.png", this->AddObject(new GameObject()));
  bg->SetScaleX(4, 4);
  go->AddComponent(bg);

  music.Open("Assets/stageState.ogg");
  music.Play();

  go = new GameObject();
  goPtr = this->AddObject(go);
  go->AddComponent(new Player(goPtr));
  go->box.x = 512;
  go->box.y = 300;
  go->AddComponent(new Collider(goPtr, {0.3, 0.3}, {64, 72}));
  Camera::GetInstance().Follow(go);

  go = new GameObject();
  goPtr = this->AddObject(go);
  go->AddComponent(new Minion(goPtr));
  go->box.x = 250;
  go->box.y = 550;
}

void State::Update(float dt)
{
  InputManager &input = InputManager::GetInstance();
  input.Update();

  Camera::GetInstance().Update(dt);

  if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY))
  {
    quitRequested = true;
  }

  for (int i = 0; i < objectArray.size(); i++)
  {
    std::weak_ptr<GameObject> go = objectArray[i];

    if (objectArray[i]->IsDead())
    {
      objectArray.erase(objectArray.begin() + i);
    }
    else
    {
      objectArray[i]->Update(dt);
    }
  }

  for (int i = 0; i < objectArray.size(); i++)
  {
    std::weak_ptr<GameObject> go = objectArray[i];
    if (auto lock = go.lock())
    {
      for (int j = i + 1; j < objectArray.size(); j++)
      {
        if (i == j)
          continue;
        std::weak_ptr<GameObject> other = objectArray[j];
        if (auto lockOther = other.lock())
        {
          std::weak_ptr<Component> collider = lock->GetComponent("Collider");
          std::weak_ptr<Component> otherCollider = lockOther->GetComponent("Collider");
          if (auto lockCollider = collider.lock())
          {
            if (auto lockOtherCollider = otherCollider.lock())
            {
              Collider *colliderComponent = (Collider *)lockCollider.get();
              Collider *otherColliderComponent = (Collider *)lockOtherCollider.get();
              if (Collision::IsColliding(colliderComponent->box, otherColliderComponent->box, lock->angle, lockOther->angle))
              {
                lock->NotifyCollision(other);
                lockOther->NotifyCollision(go);
              }
            }
          }
        }
      }
    }
  }
}

void State::Render()
{
  for (auto SharedGo : this->objectArray)
  {
    SharedGo->Render();
  }
  std::weak_ptr<SDL_Renderer> renderer = Game::GetInstance()->GetRenderer();
  SDL_RenderPresent(renderer.lock().get());
}

bool State::QuitRequested()
{
  return quitRequested;
}

void State::Start()
{
  LoadAssets();

  for (int i = 0; i < objectArray.size(); i++)
  {
    std::weak_ptr<GameObject> go = objectArray[i];
    if (auto lock = go.lock())
    {
      lock->Start();
    }
  }

  this->started = true;
}

std::weak_ptr<GameObject> State::AddObject(GameObject *go)
{
  std::shared_ptr<GameObject> sharedPtr(go);
  this->objectArray.push_back(sharedPtr);
  if (started)
  {
    go->Start();
  }
  return std::weak_ptr<GameObject>(objectArray.back());
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject *go)
{
  for (int i = 0; i < objectArray.size(); i++)
  {
    if (objectArray[i].get() == go)
    {
      return objectArray[i];
    }
  }
  return std::weak_ptr<GameObject>();
}
