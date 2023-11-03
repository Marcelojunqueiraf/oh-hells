#include "Player.hpp"

#define PI 3.14159265359

#define WALK_SPEED 5

Player *Player::player = nullptr;

Player::Player(std::weak_ptr<GameObject> associated) : Component(associated), 
speed(0, 0), linearSpeed(0), angle(0), hp(100) 
{
    stand_straight = new Sprite("Assets/Eli_front_idle.png", associated, 12, 0.1);
    stand_straight->SetScaleX(3,3);
    walk_left = new Sprite("Assets/Eli_left_walk.png", associated, 6, 0.1);
    walk_left->SetScaleX(3,3);
    walk_right = new Sprite("Assets/Eli_right_walk.png", associated, 6, 0.1);
    walk_right->SetScaleX(3,3);
    walk_back = new Sprite("Assets/Eli_back_walk.png", associated, 6, 0.1);
    walk_back->SetScaleX(3,3);
    walk_front = new Sprite("Assets/Eli_front_walk.png", associated, 6, 0.1);
    walk_front->SetScaleX(3,3);
    
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
}

Player::~Player() {
	player = nullptr;
}

void Player::Start() {
}

void Player::Update(float dt) {
    if (InputManager::GetInstance().IsKeyDown(SDLK_w)) {
        associated.lock()->box.y -= WALK_SPEED;
        stand_straight->show = false;
        walk_left->show = false;
        walk_right->show = false;
        walk_back->show = true;
        walk_front->show = false;
        if (associated.lock()->box.y < 0) associated.lock()->box.y = 0;
    } else if (InputManager::GetInstance().IsKeyDown(SDLK_s)) {
        associated.lock()->box.y += WALK_SPEED;
        stand_straight->show = false;
        walk_left->show = false;
        walk_right->show = false;
        walk_back->show = false;
        walk_front->show = true;
        if (associated.lock()->box.y > 1800) associated.lock()->box.y = 1800;
    } else if (InputManager::GetInstance().IsKeyDown(SDLK_a)) {
        associated.lock()->box.x -= WALK_SPEED;
        stand_straight->show = false;
        walk_left->show = true;
        walk_right->show = false;
        walk_back->show = false;
        walk_front->show = false;
        if (associated.lock()->box.x < 0) associated.lock()->box.x = 0;
    } else if (InputManager::GetInstance().IsKeyDown(SDLK_d)) {
        associated.lock()->box.x += WALK_SPEED;
        stand_straight->show = false;
        walk_left->show = false;
        walk_right->show = true;
        walk_back->show = false;
        walk_front->show = false;
        if (associated.lock()->box.x > 1800) associated.lock()->box.x = 1800;
    } else {
        stand_straight->show = true;
        walk_left->show = false;
        walk_right->show = false;
        walk_back->show = false;
        walk_front->show = false;
    }
}

void Player::Render() {

}

bool Player::Is(std::string type) {
	return type == "Player";
}

Vec2 Player::Position() {
	return associated.lock()->box.GetCenter();
}