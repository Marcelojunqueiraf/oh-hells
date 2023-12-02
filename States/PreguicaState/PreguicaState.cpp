#include "PreguicaState.hpp"
#include "PreguicaState_2.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Collision/Collision.cpp"
#include "../../Componentes/HealthBar/HealthBar.hpp"
#include "../../Componentes/ActionCollider/ActionCollider.hpp"
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

static std::vector<dialog_info> roteiro_luxuria_dialog =
{
 {2, "Preguiça", "Oh, tem alguem aqui."},
 {5, "Preguiça", "Tem como esperar só um pouquinho? Eu to terminando um cochilo aqui."}
};

PreguicaState::PreguicaState(): backgroundMusic(Game::GetInstance()->backgroundMusic)
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

    // Seta a camera pra ter um limite maximo de visao 
    game_view = {0, 0, (float)bg->GetWidth(), (float)bg->GetHeight()};

    // Teleporte para mapa anterior
    go = new GameObject();
    go->box = {600.0, 1550, 310, 115};
    goPtr = this->AddObject(go);
    go->AddComponent(new ActionCollider(goPtr, {1, 1}, {0, 0}, this, 
    [](State * state, std::weak_ptr<GameObject> other){
        Player *player = (Player *)other.lock()->GetComponent("Player").lock().get();
        if(player){
            state->popRequested = true;
        }
    }
    ));


    go = new GameObject();
    go->Depth = Top;
    go->box.x = 0;
    go->box.y = 0;
    goPtr = this->AddObject(go);
    preguica_dialog = new Dialog(goPtr);
    go->AddComponent(preguica_dialog);

    preguica_dialog_animation = new Sprite("Assets/preguica_dialog.png", goPtr);
    preguica_dialog_animation->SetScaleX((float)GAME_WIDTH/preguica_dialog_animation->GetWidth(), (float)GAME_HEIGHT/preguica_dialog_animation->GetHeight());
   

    go = new GameObject();
    go->Depth = Dynamic;
    player_goPtr = this->AddObject(go);
    player = new Player(player_goPtr);
    player->SetPosition(670, 1415);
    go->AddComponent(player);
    go->AddComponent(new HealthBar("Assets/barra_player.png", player_goPtr, player->GetHp()));
    go->AddComponent(new Collider(player_goPtr, {0.3, 0.3}, {64, 72}));
    player->SetView(game_view); // Seta o player pra andar em um limite espaco

    go = new GameObject();
    goPtr = this->AddObject(go);
    go->Depth = Dynamic;
    auto preg_ptr = new Preguica(goPtr, 1000);
    go->AddComponent(preg_ptr);
    go->AddComponent(new HealthBar("Assets/barra_inimiga.png", goPtr, preg_ptr->GetHp()));
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
    // backgroundMusic.Stop();
    objectArray.clear();
}

void PreguicaState::LoadAssets()
{
    // backgroundMusic.Open("Assets/Eli_memoria.ogg");
}

void PreguicaState::Update(float dt)
{

    Camera::GetInstance().Update(dt);
    dialogCooldown.Update(dt);

    auto input_manager = InputManager::GetInstance();

    quitRequested = input_manager.QuitRequested();

    // popRequested = input_manager.KeyPress(ESCAPE_KEY);

    if(!dialog_finished){
        if(dialogCooldown.Get() > dialog_time){
            if(dialog_index >= roteiro_luxuria_dialog.size()){
                dialog_finished = true;
                preguica_dialog->Hide();
            }else{
                auto& dialog_part = roteiro_luxuria_dialog[dialog_index++];
                preguica_dialog->ShowDialog(
                    preguica_dialog_animation, 
                    dialog_part.character_name,
                    dialog_part.character_msg
                );
                dialog_time = dialog_part.time;
                dialogCooldown.Restart();
            }
        }
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
    dialogCooldown.Restart();
    
    // backgroundMusic.Play();
    backgroundMusic.Resume(); // Continua a musica anterior
    Camera::GetInstance().SetView(game_view); // Seta com o tamanho da imagem
    Camera::GetInstance().Follow(player_goPtr);
}

void PreguicaState::Pause()
{
    Camera::GetInstance().Unfollow();
    backgroundMusic.Pause();
}

void PreguicaState::Resume()
{
    backgroundMusic.Resume();
    Camera::GetInstance().SetView(game_view); // Seta com o tamanho da imagem
    Camera::GetInstance().Follow(player_goPtr);
}