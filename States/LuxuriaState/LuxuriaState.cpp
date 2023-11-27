#include "LuxuriaState.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Collision/Collision.cpp"
#include "../../Componentes/Enemy/Enemy.hpp"
#include "../../Componentes/Player/Player.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"

LuxuriaState::LuxuriaState()
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

  go = new GameObject();
  goPtr = this->AddObject(go);
  go->AddComponent(new Player(goPtr));
  go->box.x = 512;
  go->box.y = 300;
  go->AddComponent(new Collider(goPtr, {0.3, 0.3}, {64, 72}));
  Camera::GetInstance().Follow(go);

  go = new GameObject();
  goPtr = this->AddObject(go);
  go->AddComponent(new Enemy(goPtr, 100));
  go->AddComponent(new Collider(goPtr, {0.3, 0.3}, {64, 72}));
  go->box.x = 300;
  go->box.y = 500;
}

LuxuriaState::~LuxuriaState()
{
    objectArray.clear();
}

void LuxuriaState::LoadAssets() {
	backgroundMusic.Open("Assets/Luxuria1.ogg");
}

void LuxuriaState::Update(float dt)
{
  
    Camera::GetInstance().Update(dt);

	auto input_manager = InputManager::GetInstance();

    quitRequested = input_manager.QuitRequested();

    popRequested = input_manager.KeyPress(ESCAPE_KEY);

    UpdateArray(dt);

    /* Verifica aqui se o jogo acabou */

    VerifyCollision();
}

void LuxuriaState::Render()
{
    for (auto &it : objectArray) {
        it->Render();
    }
}

void LuxuriaState::Start()
{
  StartArray();
  LoadAssets();
  backgroundMusic.Play();
}


void LuxuriaState::Pause() {}

void LuxuriaState::Resume() {}