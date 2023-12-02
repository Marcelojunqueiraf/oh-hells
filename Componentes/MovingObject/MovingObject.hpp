
#include "../../Game/Game.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Component/Component.hpp"
#include "../../Utils/Timer/Timer.hpp"


class MovingObject: public Component{
public:
    Timer keyCoolDown;
    MovingObject(std::string file, std::weak_ptr<GameObject> associated);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    
};


