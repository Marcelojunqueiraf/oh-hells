#include "PreguicaState.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Collision/Collision.cpp"
#include "../../Componentes/Preguica/Preguica.hpp"
#include "../../Componentes/Player/Player.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"

static std::array<Vec2, 7> pos_pilar_1 =
    {
        Vec2(360.0, -60.0),
        Vec2(1008.0, -60.0),
        Vec2(1152.0, 228.0),
        Vec2(216.0, 228.0),
        Vec2(216.0, 228.0),
        Vec2(216.0, 1164.0),
        Vec2(1152.0, 1164.0),
};

PreguicaState::PreguicaState()
{
    GameObject *go = new GameObject();
    std::weak_ptr<GameObject> goPtr = this->AddObject(go);
    Sprite *bg = new Sprite("Assets/fundo.png", goPtr);
    go->AddComponent(bg);
    go->AddComponent(new CameraFollower(goPtr));

    // Adicionando mapa
    bg = new Sprite("Assets/Cenario/mapa_boss_fight_preguica_inteiro.png", this->AddObject(new GameObject()));
    bg->SetScaleX(4, 4);
    go->AddComponent(bg);

    Rect game_view = {0, 0, (float)bg->GetWidth(), (float)bg->GetHeight()};
    Camera::GetInstance().SetView(game_view);

    go = new GameObject();
    go->Depth = Dynamic;
    goPtr = this->AddObject(go);
    auto *player = new Player(goPtr);
    go->AddComponent(player);
    go->box.x = 700;
    go->box.y = 1400;
    go->AddComponent(new Collider(goPtr, {0.3, 0.3}, {64, 72}));
    Camera::GetInstance().Follow(go);
    player->SetView(game_view);

    go = new GameObject();
    goPtr = this->AddObject(go);
    go->Depth = Dynamic;
    go->AddComponent(new Preguica(goPtr, 1000));
    go->AddComponent(new Collider(goPtr, {0.3, 0.3}, {64, 72}));
    go->box.x = 690;
    go->box.y = 100;

    go = new GameObject();
    go->Depth = Normal;
    Sprite *lago = new Sprite("Assets/Cenario/Sprite-agua_lago1.png", this->AddObject(go), 2, 0.8);
    lago->SetScaleX(4, 4);
    go->AddComponent(lago);
    go->box.x = 1080.0;
    go->box.y = 648.0;


    go = new GameObject();
    go->Depth = Dynamic;
    Sprite *tv = new Sprite("Assets/Cenario/television.png", this->AddObject(go));
    tv->SetScaleX(4, 4);
    go->AddComponent(tv);
    go->box.x = 648;
    go->box.y = 216;

    go = new GameObject();
    go->Depth = Dynamic;
    Sprite *ossos = new Sprite("Assets/Cenario/ossos_de_animal.png", this->AddObject(go));
    ossos->SetScaleX(4, 4);
    go->AddComponent(ossos);
    go->box.x = 1084;
    go->box.y = 496;

    for (auto &pos : pos_pilar_1)
    {
        go = new GameObject();
        go->Depth = Dynamic;
        Sprite *tree = new Sprite("Assets/Cenario/pilar_1.png", this->AddObject(go));
        tree->SetScaleX(4, 4);
        go->AddComponent(tree);
        go->box.x = pos.x;
        go->box.y = pos.y;
    }
}

PreguicaState::~PreguicaState()
{
    backgroundMusic.Stop();
    objectArray.clear();
}

void PreguicaState::LoadAssets()
{
    backgroundMusic.Open("Assets/Eli_memoria.ogg");
}

void PreguicaState::Update(float dt)
{

    Camera::GetInstance().Update(dt);

    auto input_manager = InputManager::GetInstance();

    quitRequested = input_manager.QuitRequested();

    popRequested = input_manager.KeyPress(ESCAPE_KEY);

    if(popRequested){
        backgroundMusic.Stop();
    }

    UpdateArray(dt);

    /* Verifica aqui se o jogo acabou */

    VerifyCollision();
}

void PreguicaState::Render()
{

    std::stable_sort(objectArray.begin()+2, objectArray.end(), [](const std::shared_ptr<GameObject> A, const std::shared_ptr<GameObject> B) 
    {
            if (A->Depth < B->Depth)
                return true;
            if (A->Depth > B->Depth)
                return false;
            if (A->Depth == Dynamic && B->Depth == Dynamic)
            {
                return A->box.y + A->box.h < B->box.y + B->box.h;
            }
            return false;
            // return A->GetLayer() < B->GetLayer();
    });
    for (auto &it : objectArray)
    {
        it->Render();
    }
}

void PreguicaState::Start()
{
    StartArray();
    LoadAssets();
    // backgroundMusic.Play();
}

void PreguicaState::Pause()
{
    backgroundMusic.Stop();
}

void PreguicaState::Resume()
{
    backgroundMusic.Stop();
}