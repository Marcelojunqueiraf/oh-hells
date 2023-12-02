#include "LuxuriaState.hpp"

#include "../../Camera/Camera.hpp"
#include "../../Utils/Collision/Collision.cpp"
#include "../../Componentes/HealthBar/HealthBar.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"
#include "../../GameObject/GameObject.hpp"

#include <algorithm>

#include <array>


// std::vector<dialog_info> luxuria_dialog_logic =
// {
//     {"Luxúria", "Ah, o meu convidado especial chegou! Fique à vontade. Quer um drink ou algo do tipo?"},

//     {"Angelus", "Não caia nas conversas dele. Eu consigo sentir o cheiro de caos daqui! Literalmente. "},

//     {"Nelli", "Deixa de ser chato Angelus. A vida é uma festa!"},

//     {"Angelus", "E não escute Nelli também. Foco! Estamos aqui para recuperar suas memórias, lembre-se disso."},

//     {"Luxúria", "Oh, querido, quer mesmo desenterrar o passado? Por que não abraça o presente? A vida é muito mais interessante quando não estamos presos àquilo que fomos."},

//     {"Luxúria", "De acordo os outros que vivem aqui, dizem que vivemos no inferno, porém não sei se concordo com essa frase. Afinal eu já trabalhei na escala 6x1 na minha vida passada."},

//     // [Animação dúvida]

//     {"Nelli", "O que diabos isso significa?"},

//     {"Angelus", "Não faço ideia."},

//     {"Luxuria", "Aquilo sim era o inferno! Desumano e terrível, eu jamais desejaria aquilo até para o meu pior inimigo. Comparado aquilo, isso aqui é o paraíso!"},

//     {"Luxúria", "Ah, lamento. Esqueci que vocês lobotomizados não fazem ideia do que eu estou falando haha."},

//     {"Luxúria", "De qualquer forma, desde que todos se explodiram eu me tornei o que sempre quis ser. "},

//     {"Luxúria", "Mas agora aqueles lá de cima querem “organizar a casa”. Faça me favor. Nem é deles para eles organizarem!"},

//     {"Luxuria", "O mundo agora é uma festa! Caos, liberdade, nenhum estresse de vidas passadas. Você não acha isso incrível?"},

//     // {, "Opção 1", "Não, eu não acho. "},

//     {"Angelus", "Não tem nada de interessante no puro caos."},

//     {"Nelli", "Chatooos~ "},

//     {"Luxúria", "É uma pena que não concorde, poderíamos nos divertir juntos. "},

//     // // {"Opção 2", "Realmente, você tem um ponto."},

//     {"Angelus", "O que?! Você não pode tá falando sério. "},

//     {"Nelli", "Haha, isso garoto! Deixa o Angelus mais irritado, por favor!"},

//     {"Luxúria", "Oh! Fico feliz que concorde comigo. É fato que o caos carrega sua própria beleza. "},

//     {"Luxúria", "Apesar do bate papo emocionante, não posso te entregar o quer, porque isso vai contra as coisas que eu quero. Afinal você vem lá de cima, não vou com a cara deles, apesar de você ser…arrumadinho. "},

//     {"Nelli", "Nossa, deprimente. Devia se cuidar mais garoto."},

//     {"Angelis", "Sinto muito, Eli."},

//     // // [Animação irritado]

//     {"Luxúria", "Enfim, sinto muito querido. Se quiser seu amuleto vai ter que pegar a força. Muita força~"},

//     // [Animação com a Gota] 

//     {"Luxúria", "Pode vir! Vou garantir que não existe nunca mais escala 6X1 e que a liberdade não seja tirada desse mundo!"},
// };

static std::array<Vec2, 20> pos_pinheiros_1 =
{
    Vec2(738, 4),
    Vec2(454, 100),
    Vec2(618, 100),
    Vec2(378, 176),
    Vec2(606, 200),
    Vec2(822, 160),
    Vec2(726.0, 268.0),
    Vec2(478.0, 228.0),
    Vec2(498.0, 360.0),
    Vec2(582.0, 412.0),
    Vec2(742.0, 388.0),
    Vec2(870.0, 444.0),
    Vec2(694.0, 560.0),
    Vec2(822.0, 636.0),
    Vec2(134.0, 396.0),
    Vec2(94.0, 540.0),
    Vec2(194.0, 568.0),
    Vec2(350.0, 732.0),
    Vec2(154.0, 812.0),
    Vec2(510.0, 840.0),
};

static std::array<Vec2, 12> pos_pinheiros_2 =
{
    Vec2(1072.0, 840.0),
    Vec2(1408.0, 736.0),
    Vec2(1532.0, 832.0),
    Vec2(1648.0, 808.0),
    Vec2(1412.0, 1112.0),
    Vec2(1732.0, 1084.0),
    Vec2(1572.0, 1288.0),
    Vec2(1504.0, 1456.0),
    Vec2(884.0, 1096.0),
    Vec2(956.0, 1328.0),
    Vec2(1184.0, 1356.0),
    Vec2(1112.0, 1560.0)
};

static std::array<Vec2, 8> pos_arv_seca_1 =
{
    Vec2(1252.0, 716.0),
    Vec2(1424.0, 880.0),
    Vec2(1252.0, 1024.0),
    Vec2(1576.0, 1040.0),
    Vec2(1124.0, 1196.0),
    Vec2(1356.0, 1412.0),
    Vec2(1420.0, 1588.0),
    Vec2(1612.0, 1580.0)
};


class MovingObject: public Component{
public:
    Timer keyCoolDown;
    MovingObject(std::string file, std::weak_ptr<GameObject> associated): Component(associated){
        Sprite * tree = new Sprite(file, associated);
        tree->SetScaleX(4,4);
        associated.lock()->AddComponent(tree);
    }

    void Update(float dt){
        keyCoolDown.Update(dt);
        InputManager &input = InputManager::GetInstance();
        if(keyCoolDown.Get() < 0.05f){
            return;
        }

        if(input.IsKeyDown(SDLK_SPACE)){
            Rect pos = associated.lock()->box;
            printf("%0.1lf, %0.1lf\n", pos.x, pos.y);
        }

        keyCoolDown.Restart();

        bool up = input.IsKeyDown(SDLK_w);
        bool down = input.IsKeyDown(SDLK_s);
        bool left = input.IsKeyDown(SDLK_a);
        bool right = input.IsKeyDown(SDLK_d);

        int walk = 4;

        if(input.IsKeyDown(SDLK_LSHIFT)){
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

    void Render(){}

    bool Is(std::string type){
        return type == "Arvore";
    }
    
};




LuxuriaState::LuxuriaState()
{
    GameObject *go = new GameObject();
    std::weak_ptr<GameObject> goPtr = this->AddObject(go);
    Sprite *bg = new Sprite("Assets/fundo.png", goPtr);
    go->AddComponent(bg);
    go->AddComponent(new CameraFollower(goPtr));


    // Adicionando mapa
    bg = new Sprite("Assets/Cenario/mapa_portal_preguica.png", this->AddObject(new GameObject()));
    bg->SetScaleX(4, 4);
    go->AddComponent(bg);

    // Seta a camera pra ter um limite maximo de visao 
    Rect game_view = {0,0, bg->GetWidth(), bg->GetHeight()};
    Camera::GetInstance().SetView(game_view); // Seta com o tamanho da imagem


    go = new GameObject();
    go->Depth = Dynamic;
    auto player_goPtr = this->AddObject(go);
    player = new Player(player_goPtr);
    go->AddComponent(player);
    go->AddComponent(new HealthBar("Assets/barra_player.png", player_goPtr, player->GetHp()));
    go->AddComponent(new Collider(player_goPtr, {0.3, 0.3}, Vec2(64, 72)));
    Camera::GetInstance().Follow(go);
    go->box.x = 512;
    go->box.y = 300;

    // Seta o player pra andar em um limite espaco
    player->SetView(game_view);


    go = new GameObject();
    go->Depth = Top;
    go->box.x = 0;
    go->box.y = 0;
    goPtr = this->AddObject(go);
    luxuria_dialog = new Dialog(goPtr);
    luxuria_dialog_animation = new Sprite("Assets/luxuria_dialog.png", goPtr);
    luxuria_dialog_animation->SetScaleX((float)GAME_WIDTH/luxuria_dialog_animation->GetWidth(), (float)GAME_HEIGHT/luxuria_dialog_animation->GetHeight());
    go->AddComponent(luxuria_dialog);
    luxuria_dialog->ShowDialog(luxuria_dialog_animation, {"Luxúria", "Ah, o meu convidado especial chegou! Fique à vontade. Quer um drink ou algo do tipo?"});


    go = new GameObject();
    go->Depth = Dynamic;
    auto luxuria_goPtr = this->AddObject(go);
    luxuria = new Luxuria(luxuria_goPtr, 100, player_goPtr);
    go->AddComponent(luxuria);
    go->AddComponent(new HealthBar("Assets/barra_inimiga.png", luxuria_goPtr, luxuria->GetHp()));
    go->AddComponent(new Collider(luxuria_goPtr, {0.3, 0.3}, Vec2(64, 72)));
    go->box.x = 300;
    go->box.y = 500;

    // go = new GameObject();
    // go->Depth = Dynamic;
    // go->AddComponent(new MovingObject("Assets/Cenario/portal.png",this->AddObject(go)));
    // go->box.x = 800;
    // go->box.y = 800;
    // Camera::GetInstance().Follow(go);

    go = new GameObject();
    go->Depth = Dynamic;
    Sprite * tree = new Sprite("Assets/Cenario/portal.png", this->AddObject(go));
    tree->SetScaleX(4,4);
    go->AddComponent(tree);
    go->box.x = 4;
    go->box.y = 56;

    for(auto & pos: pos_pinheiros_2){
        go = new GameObject();
        go->Depth = Dynamic;
        Sprite * tree = new Sprite("Assets/Cenario/arvore_1.png", this->AddObject(go));
        tree->SetScaleX(4,4);
        go->AddComponent(tree);
        go->box.x = pos.x - 82;
        go->box.y = pos.y - 108;
    }

    for(auto & pos: pos_pinheiros_2){
        go = new GameObject();
        go->Depth = Dynamic;
        Sprite * tree = new Sprite("Assets/Cenario/arvore_2.png", this->AddObject(go));
        tree->SetScaleX(4,4);
        go->AddComponent(tree);
        go->box.x = pos.x;
        go->box.y = pos.y;
    }

    for(auto & pos: pos_arv_seca_1){
        go = new GameObject();
        go->Depth = Dynamic;
        Sprite * tree = new Sprite("Assets/Cenario/arvore_seca_2.png", this->AddObject(go));
        tree->SetScaleX(4,4);
        go->AddComponent(tree);
        go->box.x = pos.x;
        go->box.y = pos.y;
    }


}

LuxuriaState::~LuxuriaState()
{
    objectArray.clear();
}

void LuxuriaState::LoadAssets()
{
    backgroundMusic.Open("Assets/Luxuria1.ogg");
}

void LuxuriaState::Update(float dt)
{

    Camera::GetInstance().Update(dt);

    auto input_manager = InputManager::GetInstance();

    quitRequested = input_manager.QuitRequested();

    popRequested = input_manager.KeyPress(ESCAPE_KEY);

    if (popRequested)
        backgroundMusic.Stop();

    UpdateArray(dt);

    /* Verifica aqui se o jogo acabou */
    if(luxuria->GetHp() <= 0){
        backgroundMusic.Stop();
    }

    // if(input_manager.KeyPress(SDLK_0)){
    // }
    VerifyCollision();
}

void LuxuriaState::Render()
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

void LuxuriaState::Start()
{
    StartArray();
    LoadAssets();
    backgroundMusic.Play();
}

void LuxuriaState::Pause()
{
    backgroundMusic.Stop();
}

void LuxuriaState::Resume()
{
    backgroundMusic.Play();
}