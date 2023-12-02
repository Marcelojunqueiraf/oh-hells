#include "MovingObject.hpp"

MovingObject::MovingObject(std::string file, std::weak_ptr<GameObject> associated): Component(associated){
    Sprite * tree = new Sprite(file, associated);
    tree->SetScaleX(4,4);
    associated.lock()->AddComponent(tree);
}

void MovingObject::Update(float dt){
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

void MovingObject::Render(){}

bool MovingObject::Is(std::string type){
    return type == "Arvore";
}

