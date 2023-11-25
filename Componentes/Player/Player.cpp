#include "Player.hpp"

#define DASH_SPEED 60
#define WALK_SPEED 5
#define ATTACK_SPEED 0.7f

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
    left_attack_animation = new Sprite("Assets/Eli_left_attack.png", associated, 7, ATTACK_SPEED/7);
    left_attack_animation->SetScaleX(3, 3);
    right_attack_animation = new Sprite("Assets/Eli_right_attack.png", associated, 7, ATTACK_SPEED/7);
    right_attack_animation->SetScaleX(3, 3);
    back_attack_animation = new Sprite("Assets/Eli_back_attack.png", associated, 7, ATTACK_SPEED/7);
    back_attack_animation->SetScaleX(3, 3);
    front_attack_animation = new Sprite("Assets/Eli_front_attack.png", associated, 7, ATTACK_SPEED/7);
    front_attack_animation->SetScaleX(3, 3);

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

    associated.lock()->AddComponent(stand_straight);
    associated.lock()->AddComponent(walk_left);
    associated.lock()->AddComponent(walk_right);
    associated.lock()->AddComponent(walk_back);
    associated.lock()->AddComponent(walk_front);
    associated.lock()->AddComponent(hit_animation);
    associated.lock()->AddComponent(left_attack_animation);
    associated.lock()->AddComponent(right_attack_animation);
    associated.lock()->AddComponent(front_attack_animation);
    associated.lock()->AddComponent(back_attack_animation);
    player = this;
    last_animation = stand_straight;
    attackCooldown = Timer();
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

void Player::ShowSprite(Sprite * spr){
    last_animation->show = false;
    spr->show = true;
    last_animation = spr;
}

void Player::Update(float dt)
{
    hitTimer.Update(dt);
    shootCooldown.Update(dt);
    attackCooldown.Update(dt);
    

    if (hitTimer.Get() < 0.4f)
    {
        return;
    }

    if(dashed && attackCooldown.Get() > 0.2f){
        dashed = false;
        associated.lock()->box += dash_direction;
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
        Bullet *bullet = new Bullet(bulletPtr, angle, 500, 10, 1000, "Assets/Eli_bullet.png", false);
        bulletGO->AddComponent(bullet);
        shootCooldown.Restart();
    }

    if(input.IsKeyDown(SDLK_SPACE) && attackCooldown.Get() > ATTACK_SPEED){

        dash_direction = {0,0};

        if (left){
            dash_direction.x -= DASH_SPEED;
            ShowSprite(left_attack_animation);
        }
        else if (right){
            dash_direction.x += DASH_SPEED;
            ShowSprite(right_attack_animation);
        }
        else if(up) {
            dash_direction.y -= DASH_SPEED;
            ShowSprite(back_attack_animation);
        }
        else{
            dash_direction.y += DASH_SPEED;
            ShowSprite(front_attack_animation);
        }

        dashed = true;
        attackCooldown.Restart();
        sword_attack_sound->Play();
    }
    else if ((up || down || left || right) && attackCooldown.Get() > ATTACK_SPEED)
    {
        if (up)
        {
            associated.lock()->box.y -= WALK_SPEED;
            ShowSprite(walk_back);
            if (associated.lock()->box.x < 0)
                associated.lock()->box.x = 0;
        }
        else if (down)
        {
            associated.lock()->box.y += WALK_SPEED;
            ShowSprite(walk_front);
            if (associated.lock()->box.y > 1800)
                associated.lock()->box.y = 1800;
        }

        if (left)
        {
            associated.lock()->box.x -= WALK_SPEED;
            ShowSprite(walk_left);

            if (associated.lock()->box.x < 0)
                associated.lock()->box.x = 0;
        }
        else if (right)
        {
            associated.lock()->box.x += WALK_SPEED;
            ShowSprite(walk_right);

            if (associated.lock()->box.x > 1800)
                associated.lock()->box.x = 1800;
        }
    }
    else
    {
        if(attackCooldown.Get() < 0.7f){
            return;
        }
        ShowSprite(stand_straight);
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

        ShowSprite(hit_animation);
        hitTimer.Restart();
        hit_sound->Play();
    }
}