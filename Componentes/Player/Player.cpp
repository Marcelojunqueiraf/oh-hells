#include "Player.hpp"

#define PI 3.14159265359

#define WALK_SPEED 5

Player *Player::player = nullptr;

static Sprite *last_animation = nullptr;

Player::Player(std::weak_ptr<GameObject> associated) : Component(associated),
                                                       speed(0, 0), linearSpeed(0), angle(0), hp(100)
{
    stand_straight = new Sprite("Assets/Eli_front_idle.png", associated, 12, 0.1);
    stand_straight->SetScaleX(3, 3);
    walk_left = new Sprite("Assets/Eli_left_walk.png", associated, 6, 0.1);
    walk_left->SetScaleX(3, 3);
    walk_right = new Sprite("Assets/Eli_right_walk.png", associated, 6, 0.1);
    walk_right->SetScaleX(3, 3);
    walk_back = new Sprite("Assets/Eli_back_walk.png", associated, 6, 0.1);
    walk_back->SetScaleX(3, 3);
    walk_front = new Sprite("Assets/Eli_front_walk.png", associated, 6, 0.1);
    walk_front->SetScaleX(3, 3);

    walk_left->show = false;
    walk_right->show = false;
    walk_back->show = false;
    walk_front->show = false;
    associated.lock()->AddComponent(stand_straight);
    associated.lock()->AddComponent(walk_left);
    associated.lock()->AddComponent(walk_right);
    associated.lock()->AddComponent(walk_back);
    associated.lock()->AddComponent(walk_front);
    player = this;
    last_animation = stand_straight;
}

Player::~Player()
{
    player = nullptr;
}

void Player::Start()
{
}

void Player::Update(float dt)
{

    bool up = InputManager::GetInstance().IsKeyDown(SDLK_w);
    bool down = InputManager::GetInstance().IsKeyDown(SDLK_s);
    bool left = InputManager::GetInstance().IsKeyDown(SDLK_a);
    bool right = InputManager::GetInstance().IsKeyDown(SDLK_d);

    if (up | down | left | right)
    {
        if (up)
        {
            associated.lock()->box.y -= WALK_SPEED;
            last_animation->show = false;
            walk_back->show = true;
            last_animation = walk_back;
            if (associated.lock()->box.x < 0)
                associated.lock()->box.x = 0;
        }
        else if (down)
        {
            associated.lock()->box.y += WALK_SPEED;
            last_animation->show = false;
            walk_front->show = true;
            last_animation = walk_front;
            if (associated.lock()->box.y > 1800)
                associated.lock()->box.y = 1800;
        }

        if (left)
        {
            associated.lock()->box.x -= WALK_SPEED;
            last_animation->show = false;
            walk_left->show = true;
            last_animation = walk_left;

            if (associated.lock()->box.x < 0)
                associated.lock()->box.x = 0;
        }
        else if (right)
        {
            associated.lock()->box.x += WALK_SPEED;
            last_animation->show = false;
            walk_right->show = true;
            last_animation = walk_right;

            if (associated.lock()->box.x > 1800)
                associated.lock()->box.x = 1800;
        }
    }
    else
    {
        last_animation->show = false;
        stand_straight->show = true;
        last_animation = stand_straight;
    }
}

void Player::Render()
{
}

bool Player::Is(std::string type)
{
    return type == "Player";
}

Vec2 Player::Position()
{
    return associated.lock()->box.GetCenter();
}