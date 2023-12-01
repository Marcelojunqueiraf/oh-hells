#pragma once

#include "../../Game/Game.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../Dialog/Dialog.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../RegularBullet/RegularBullet.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>

class Sprite;

class Luxuria : public Component
{

public:
    Sprite * health_bar;
    Luxuria(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go, Dialog * luxuria_dialog);
    ~Luxuria();
    void Update(float dt);
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    void ShowSprite(Sprite *spr);
    int GetHp();
    void TakeDamage(int damage);

private:
    int hp;
    std::weak_ptr<GameObject> player_go;
    Dialog * luxuria_dialog;

    Sprite *idle_animation, *hit_animation, *dialog_animation;
    Timer shootCooldown;
    Timer hitTimer;

    std::vector<dialog_info> luxuria_dialog_logic =
    {
        {"Luxúria", "Ah, o meu convidado especial chegou! Fique à vontade. Quer um drink ou algo do tipo?"},

        {"Angelus", "Não caia nas conversas dele. Eu consigo sentir o cheiro de caos daqui! Literalmente. "},

        {"Nelli", "Deixa de ser chato Angelus. A vida é uma festa!"},

        {"Angelus", "E não escute Nelli também. Foco! Estamos aqui para recuperar suas memórias, lembre-se disso."},

        {"Luxúria", "Oh, querido, quer mesmo desenterrar o passado? Por que não abraça o presente? A vida é muito mais interessante quando não estamos presos àquilo que fomos."},

        {"Luxúria", "De acordo os outros que vivem aqui, dizem que vivemos no inferno, porém não sei se concordo com essa frase. Afinal eu já trabalhei na escala 6x1 na minha vida passada."},

        // [Animação dúvida]

        {"Nelli", "O que diabos isso significa?"},

        {"Angelus", "Não faço ideia."},

        {"Luxuria", "Aquilo sim era o inferno! Desumano e terrível, eu jamais desejaria aquilo até para o meu pior inimigo. Comparado aquilo, isso aqui é o paraíso!"},

        {"Luxúria", "Ah, lamento. Esqueci que vocês lobotomizados não fazem ideia do que eu estou falando haha."},

        {"Luxúria", "De qualquer forma, desde que todos se explodiram eu me tornei o que sempre quis ser. "},

        {"Luxúria", "Mas agora aqueles lá de cima querem “organizar a casa”. Faça me favor. Nem é deles para eles organizarem!"},

        {"Luxuria", "O mundo agora é uma festa! Caos, liberdade, nenhum estresse de vidas passadas. Você não acha isso incrível?"},

        // {, "Opção 1", "Não, eu não acho. "},

        {"Angelus", "Não tem nada de interessante no puro caos."},

        {"Nelli", "Chatooos~ "},

        {"Luxúria", "É uma pena que não concorde, poderíamos nos divertir juntos. "},

        // // {"Opção 2", "Realmente, você tem um ponto."},

        {"Angelus", "O que?! Você não pode tá falando sério. "},

        {"Nelli", "Haha, isso garoto! Deixa o Angelus mais irritado, por favor!"},

        {"Luxúria", "Oh! Fico feliz que concorde comigo. É fato que o caos carrega sua própria beleza. "},

        {"Luxúria", "Apesar do bate papo emocionante, não posso te entregar o quer, porque isso vai contra as coisas que eu quero. Afinal você vem lá de cima, não vou com a cara deles, apesar de você ser…arrumadinho. "},

        {"Nelli", "Nossa, deprimente. Devia se cuidar mais garoto."},

        {"Angelis", "Sinto muito, Eli."},

        // // [Animação irritado]

        {"Luxúria", "Enfim, sinto muito querido. Se quiser seu amuleto vai ter que pegar a força. Muita força~"},

        // [Animação com a Gota] 

        {"Luxúria", "Pode vir! Vou garantir que não existe nunca mais escala 6X1 e que a liberdade não seja tirada desse mundo!"},
    };
};