#include "Player.hpp"

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
    hit_animation = new Sprite("Assets/Eli_front_hit.png", associated, 4, 0.1);
    hit_animation->SetScaleX(3, 3);

    walk_left->show = false;
    walk_right->show = false;
    walk_back->show = false;
    walk_front->show = false;
    hit_animation->show = false;
    associated.lock()->AddComponent(stand_straight);
    associated.lock()->AddComponent(walk_left);
    associated.lock()->AddComponent(walk_right);
    associated.lock()->AddComponent(walk_back);
    associated.lock()->AddComponent(walk_front);
    associated.lock()->AddComponent(hit_animation);
    player = this;
    last_animation = stand_straight;
    shootCooldown = Timer();
    hitTimer = Timer();
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
    hitTimer.Update(dt);
    shootCooldown.Update(dt);

    if (hitTimer.Get() < 0.4f)
    {
        return;
    }

    if (hp <= 0)
    {
        associated.lock()->RequestDelete();
    }

    InputManager &input = InputManager::GetInstance();

    bool up = input.IsKeyDown(SDLK_w);
    bool down = input.IsKeyDown(SDLK_s);
    bool left = input.IsKeyDown(SDLK_a);
    bool right = input.IsKeyDown(SDLK_d);

    if (shootCooldown.Get() > 0.3f && input.IsMouseDown(1))
    {
        GameObject *bulletGO = new GameObject();
        bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
        bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
        std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().lock()->AddObject(bulletGO);

        // get mouse direction
        Vec2 distance = Vec2(input.GetMouseX(), input.GetMouseY()) - this->associated.lock()->box.GetCenter();

        float angle = distance.getAngle();
        Bullet *bullet = new Bullet(bulletPtr, angle, 500, 10, 1000, "Assets/minionbullet2.png", false);
        bulletGO->AddComponent(bullet);
        shootCooldown.Restart();
    }

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

void Player::NotifyCollision(std::weak_ptr<GameObject> other)
{
    Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
    if (bullet != nullptr && bullet->targetPlayer)
    {
        hp -= bullet->GetDamage();
        last_animation->show = false;
        hit_animation->show = true;
        last_animation = hit_animation;
        hitTimer.Restart();
    }
}