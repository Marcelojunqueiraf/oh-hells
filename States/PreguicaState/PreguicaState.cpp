#include "PreguicaState.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Collision/Collision.cpp"
#include "../../Componentes/Preguica/Preguica.hpp"
#include "../../Componentes/Player/Player.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"

class MovingObject : public Component
{
public:
    Timer keyCoolDown;
    MovingObject(std::string file, std::weak_ptr<GameObject> associated) : Component(associated)
    {
        Sprite *tree = new Sprite(file, associated);
        tree->SetScaleX(4, 4);
        associated.lock()->AddComponent(tree);
    }

    void Update(float dt)
    {
        keyCoolDown.Update(dt);
        InputManager &input = InputManager::GetInstance();
        if (keyCoolDown.Get() < 0.05f)
        {
            return;
        }

        if (input.IsKeyDown(SDLK_SPACE))
        {
            Rect pos = associated.lock()->box;
            printf("%0.1lf, %0.1lf\n", pos.x, pos.y);
        }

        keyCoolDown.Restart();

        bool up = input.IsKeyDown(SDLK_w);
        bool down = input.IsKeyDown(SDLK_s);
        bool left = input.IsKeyDown(SDLK_a);
        bool right = input.IsKeyDown(SDLK_d);

        int walk = 4;

        if (input.IsKeyDown(SDLK_LSHIFT))
        {
            walk = 12;
        }

        if (up)
        {
            associated.lock()->box.y -= walk;
        }
        else if (down)
        {
            associated.lock()->box.y += walk;
        }
        else if (left)
        {
            associated.lock()->box.x -= walk;
        }
        else if (right)
        {
            associated.lock()->box.x += walk;
        }
    }

    void Render() {}

    bool Is(std::string type)
    {
        return type == "Arvore";
    }
};

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
    go->box.x = 712;
    go->box.y = 1500;
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

    // go = new GameObject();
    // go->Depth = Dynamic;
    // go->AddComponent(new MovingObject("Assets/Cenario/Sprite-agua_lago1.png",this->AddObject(go)));
    // go->box.x = 800;
    // go->box.y = 800;
    // Camera::GetInstance().Follow(go);


    go = new GameObject();
    go->Depth = Normal;
    Sprite *lago = new Sprite("Assets/Cenario/Sprite-agua_lago1.png", this->AddObject(go), 2, 0.8);
    lago->SetScaleX(4, 4);
    go->AddComponent(lago);
    go->box.x = 1080.0;
    go->box.y = 648.0;

    go = new GameObject();
    go->Depth = Dynamic;
    Sprite *ossos = new Sprite("Assets/Cenario/ossos_de_animal.png", this->AddObject(go));
    ossos->SetScaleX(4, 4);
    go->AddComponent(ossos);
    go->box.x = 1084.0;
    go->box.y = 496.0;

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