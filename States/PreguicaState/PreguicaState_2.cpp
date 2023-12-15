#include "PreguicaState_2.hpp"
#include "PreguicaState.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Collision/Collision.cpp"
#include "../../Componentes/HealthBar/HealthBar.hpp"
#include "../../Componentes/ActionCollider/ActionCollider.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"
#include "../../GameObject/GameObject.hpp"
#include "../../Componentes/MovingObject/MovingObject.hpp"

#include <algorithm>

#include <array>

static std::array<Vec2, 21> pos_pinheiros_1 =
    {
        Vec2(268.0, 844.0),
        Vec2(268.0, 844.0),
        Vec2(1288.0, 716.0),
        Vec2(1384.0, 872.0),
        Vec2(1384.0, 872.0),
        Vec2(1280.0, 936.0),
        Vec2(1280.0, 936.0),
        Vec2(1448.0, 976.0),
        Vec2(1220.0, 1016.0),
        Vec2(988.0, 1048.0),
        Vec2(1092.0, 1112.0),
        Vec2(916.0, 1148.0),
        Vec2(916.0, 1148.0),
        Vec2(1184.0, 1236.0),
        Vec2(1484.0, 1304.0),
        Vec2(748.0, 1296.0),
        Vec2(644.0, 1240.0),
        Vec2(536.0, 1304.0),
        Vec2(368.0, 1296.0),
        Vec2(188.0, 1296.0),
        Vec2(-32.0, 1300.0)};

static std::array<Vec2, 18> pos_arv_seca_1 =
    {
        Vec2(504.0, 584.0),
        Vec2(552.0, 732.0),
        Vec2(552.0, 732.0),
        Vec2(1040.0, 428.0),
        Vec2(1008.0, 736.0),
        Vec2(1008.0, 736.0),
        Vec2(1092.0, 840.0),
        Vec2(1508.0, 704.0),
        Vec2(1364.0, 1152.0),
        Vec2(1416.0, 1332.0),
        Vec2(1020.0, 1312.0),
        Vec2(1020.0, 1312.0),
        Vec2(800.0, 1076.0),
        Vec2(664.0, 1336.0),
        Vec2(352.0, 1184.0),
        Vec2(352.0, 1184.0),
        Vec2(100.0, 1328.0),
        Vec2(76.0, 856.0)};

PreguicaState_2::PreguicaState_2()
{
    GameObject *go = new GameObject();
    std::weak_ptr<GameObject> goPtr = this->AddObject(go);
    Sprite *bg = new Sprite("Assets/fundo.png", goPtr);
    go->AddComponent(bg);
    go->AddComponent(new CameraFollower(goPtr));

    // Adicionando mapa
    bg = new Sprite("Assets/Cenario/mapa_preguica_2.png", this->AddObject(new GameObject()));
    bg->SetScaleX(4, 4);
    go->AddComponent(bg);

    // Seta a camera pra ter um limite maximo de visao
    game_view = {0, 0, (float)bg->GetWidth(), (float)bg->GetHeight()};

    // Teleporte para mapa anterior
    go = new GameObject();
    go->box = {0, 1060, 10, 285};
    goPtr = this->AddObject(go);
    go->AddComponent(new ActionCollider(goPtr, {1, 1}, {0, 0}, this,
                                        [](State *state, std::weak_ptr<GameObject> other)
                                        {
                                            Player *player = (Player *)other.lock()->GetComponent("Player").lock().get();
                                            if (player)
                                            {
                                                state->popRequested = true;
                                            }
                                        }));

    // Teleporte de mapa
    go = new GameObject();
    go->box = {455, 550, 480, 60};
    goPtr = this->AddObject(go);
    go->AddComponent(new ActionCollider(goPtr, {1, 1}, {0, 0}, this,
                                        [](State *state, std::weak_ptr<GameObject> other)
                                        {
                                            Player *player = (Player *)other.lock()->GetComponent("Player").lock().get();
                                            if (player)
                                            {
                                                player->SetPosition(645, 545);
                                                Game::GetInstance()->Push(new PreguicaState());
                                            }
                                        }));

    go = new GameObject();
    go->Depth = Dynamic;
    player_goPtr = this->AddObject(go);
    player = new Player(player_goPtr);
    player->SetPosition(-45, 1105);
    go->AddComponent(player);
    go->AddComponent(new HealthBar(player_goPtr, player->GetHp(), player->GetHp()));
    go->AddComponent(new Collider(player_goPtr, {0.3, 0.3}, Vec2(64, 72)));
    player->SetView(game_view); // Seta o player pra andar em um limite espaco

    // go = new GameObject();
    // go->Depth = Dynamic;
    // Sprite * tree = new Sprite("Assets/Cenario/portal.png", this->AddObject(go));
    // tree->SetScaleX(4,4);
    // go->AddComponent(tree);
    // go->box.x = 4;
    // go->box.y = 56;

    for (auto &pos : pos_pinheiros_1)
    {
        go = new GameObject();
        go->Depth = Dynamic;
        Sprite *tree = new Sprite("Assets/Cenario/arvore_2.png", this->AddObject(go));
        tree->SetScaleX(4, 4);
        go->AddComponent(tree);
        go->box.x = pos.x;
        go->box.y = pos.y;
    }

    for (auto &pos : pos_arv_seca_1)
    {
        go = new GameObject();
        go->Depth = Dynamic;
        Sprite *tree = new Sprite("Assets/Cenario/arvore_seca_3.png", this->AddObject(go));
        tree->SetScaleX(4, 4);
        go->AddComponent(tree);
        go->box.x = pos.x;
        go->box.y = pos.y;
    }
}

PreguicaState_2::~PreguicaState_2()
{
    objectArray.clear();
}

void PreguicaState_2::LoadAssets()
{
}

void PreguicaState_2::Update(float dt)
{

    Camera::GetInstance().Update(dt);

    auto input_manager = InputManager::GetInstance();

    quitRequested = input_manager.QuitRequested();

    // popRequested = input_manager.KeyPress(ESCAPE_KEY);

    UpdateArray(dt);

    /* Verifica aqui se o jogo acabou */

    VerifyCollision();
}

void PreguicaState_2::Render()
{

    std::stable_sort(objectArray.begin() + 2, objectArray.end(), [](const std::shared_ptr<GameObject> A, const std::shared_ptr<GameObject> B)
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

void PreguicaState_2::Start()
{
    StartArray();
    LoadAssets();
    Camera::GetInstance().SetView(game_view); // Seta com o tamanho da imagem
    Camera::GetInstance().Follow(player_goPtr);
}

void PreguicaState_2::Pause()
{
    // Game::PauseMusic();
    Camera::GetInstance().Unfollow();
}

void PreguicaState_2::Resume()
{
    // Game::ResumeMusic();
    Camera::GetInstance().SetView(game_view); // Seta com o tamanho da imagem
    Camera::GetInstance().Follow(player_goPtr);
}