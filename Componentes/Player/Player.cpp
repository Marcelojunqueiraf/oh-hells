#include "Player.hpp"

#define DASH_SPEED 500
#define WALK_SPEED 200
#define ATTACK_SPEED 0.7f
#define ATTACK_DISTANCE 350

#define SCALE 3
#define SPRITE_WIDTH SCALE * 64
#define SPRITE_HEIGHT SCALE * 64
#define SPRITE_WHITE_SPACE SCALE * 24

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
    left_attack_animation = new Sprite("Assets/Eli_left_attack.png", associated, 7, ATTACK_SPEED / 7);
    left_attack_animation->SetScaleX(3, 3);
    right_attack_animation = new Sprite("Assets/Eli_right_attack.png", associated, 7, ATTACK_SPEED / 7);
    right_attack_animation->SetScaleX(3, 3);
    back_attack_animation = new Sprite("Assets/Eli_back_attack.png", associated, 7, ATTACK_SPEED / 7);
    back_attack_animation->SetScaleX(3, 3);
    front_attack_animation = new Sprite("Assets/Eli_front_attack.png", associated, 7, ATTACK_SPEED / 7);
    front_attack_animation->SetScaleX(3, 3);
    death_animation = new Sprite("Assets/Eli_death.png", associated, 8, 0.1);
    death_animation->SetScaleX(3, 3);

    hit_sound = new Sound("Assets/dano.ogg", associated);
    hit_sound->Volume(16);
    sword_attack_sound = new Sound("Assets/espada.ogg", associated);

    walk_left->show = false;
    walk_right->show = false;
    walk_back->show = false;
    walk_front->show = false;

    left_attack_animation->show = false;
    right_attack_animation->show = false;
    back_attack_animation->show = false;
    front_attack_animation->show = false;

    hit_animation->show = false;
    death_animation->show = false;

    associated.lock()->AddComponent(stand_straight);
    associated.lock()->AddComponent(walk_left);
    associated.lock()->AddComponent(walk_right);
    associated.lock()->AddComponent(walk_back);
    associated.lock()->AddComponent(walk_front);
    associated.lock()->AddComponent(left_attack_animation);
    associated.lock()->AddComponent(right_attack_animation);
    associated.lock()->AddComponent(front_attack_animation);
    associated.lock()->AddComponent(back_attack_animation);
    associated.lock()->AddComponent(hit_animation);
    associated.lock()->AddComponent(death_animation);
    last_animation = stand_straight;
    attackCooldown = Timer();
    shootCooldown = Timer();
    hitTimer = Timer();
    dashTimer = Timer();
}

Player::~Player()
{
}

void Player::Start()
{
}

void Player::ShowSprite(Sprite *spr)
{
    last_animation->show = false;
    spr->show = true;
    last_animation = spr;
}

void Player::SetPosition(int x, int y)
{
    Rect &player_pos = associated.lock()->box;
    player_pos.x = x;
    player_pos.y = y;
}

void Player::SetView(Rect max_view)
{
    this->max_view = max_view;
}

void Player::Update(float dt)
{
    hitTimer.Update(dt);
    shootCooldown.Update(dt);
    attackCooldown.Update(dt);
    dashTimer.Update(dt);

    InputManager &input = InputManager::GetInstance();

    bool up = input.IsKeyDown(SDLK_w);
    bool down = input.IsKeyDown(SDLK_s);
    bool left = input.IsKeyDown(SDLK_a);
    bool right = input.IsKeyDown(SDLK_d);

    // if (input.IsKeyDown(SDLK_SPACE))
    // {
    //     Rect pos = associated.lock()->box;
    //     printf("%0.1lf, %0.1lf\n", pos.x, pos.y);
    // }

    Rect &player_pos = associated.lock()->box;

    switch (state)
    {
    case (RESTING):

        if (shootCooldown.Get() > 0.3f && input.IsMouseDown(1))
        {

            GameObject *bulletGO = new GameObject();
            bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
            bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
            std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

            Vec2 target = Vec2(input.GetMouseX(), input.GetMouseY());
            Vec2 origin = this->associated.lock()->box.GetCenter();

            Vec2 distance = target - origin;
            distance = distance.normalize();
            target = origin + distance * ATTACK_DISTANCE;

            Boomerang *bullet = new Boomerang(bulletPtr, target, 500, 10, "Assets/Eli_bullet.png", this->associated, false);
            bulletGO->AddComponent(bullet);
            shootCooldown.Restart();
        }

        if (input.IsKeyDown(SDLK_SPACE) && dashTimer.Get() > 1.0f)
        {

            state = DASHING;
            dash_direction = {0, 0};
            if (up)
            {
                dash_direction.y = -1;
                ShowSprite(back_attack_animation);
            }
            else if (down)
            {
                dash_direction.y = 1;
                ShowSprite(front_attack_animation);
            }

            if (left)
            {
                dash_direction.x = -1;
                ShowSprite(left_attack_animation);
            }
            else if (right)
            {
                dash_direction.x = 1;
                ShowSprite(right_attack_animation);
            }
            dash_direction = dash_direction.normalize() * DASH_SPEED;
            sword_attack_sound->Play();
            dashTimer.Restart();
        }
        else if ((up || down || left || right) && attackCooldown.Get() > ATTACK_SPEED)
        {
            Vec2 velocity = {0, 0};
            if (up)
            {
                velocity.y = -1;
                ShowSprite(walk_back);
            }
            else if (down)
            {
                velocity.y = 1;
                ShowSprite(walk_front);
            }

            if (left)
            {
                velocity.x = -1;
                ShowSprite(walk_left);
            }
            else if (right)
            {
                velocity.x = 1;
                ShowSprite(walk_right);
            }
            velocity = velocity.normalize() * WALK_SPEED * dt;

            associated.lock()->box += velocity;
        }
        else
        {
            ShowSprite(stand_straight);
        }
        break;

    case (MOVING):
        state = RESTING;

    case (SHOOTING):
        state = RESTING;

    case (DASHING):
        if (dashTimer.Get() < 0.3f)
            associated.lock()->box += dash_direction * dt;
        else
        {
            state = RESTING;
        }
        break;

    case (HITED):
        if (hitTimer.Get() > 0.4f)
            state = RESTING;
        break;
    case (DEAD):
        if (hitTimer.Get() > 0.79f)
            associated.lock()->RequestDelete();
        break;

    default: // State Default = RESTING
        state = RESTING;
        ShowSprite(stand_straight);
        break;
    }

    player_pos.x = (player_pos.x < max_view.x - SPRITE_WHITE_SPACE) ? max_view.x - SPRITE_WHITE_SPACE : player_pos.x;

    player_pos.x = (player_pos.x > max_view.w - SPRITE_WIDTH + SPRITE_WHITE_SPACE) ? max_view.w - SPRITE_WIDTH + SPRITE_WHITE_SPACE : player_pos.x;

    player_pos.y = (player_pos.y < max_view.y - SPRITE_WHITE_SPACE) ? max_view.y - SPRITE_WHITE_SPACE : player_pos.y;

    player_pos.y = (player_pos.y > max_view.h - SPRITE_HEIGHT + SPRITE_WHITE_SPACE) ? max_view.h - SPRITE_HEIGHT + SPRITE_WHITE_SPACE : player_pos.y;
}

int &Player::GetHp()
{
    return hp;
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
        hitTimer.Restart();
        hit_sound->Play();

        if (hp <= 0)
        {
            state = DEAD;
            ShowSprite(death_animation);
        }
        else
        {
            state = HITED;
            ShowSprite(hit_animation);
        }
    }
}