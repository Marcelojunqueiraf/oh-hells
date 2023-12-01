#include "TitleState.hpp"
#include "../LuxuriaState/LuxuriaState.hpp"
#include "../PreguicaState/PreguicaState.hpp"
#include "../../Game/Game.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Componentes/Sprite/Sprite.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"

TitleState::TitleState() {
    auto bg = new GameObject;
    auto bg_ptr = AddObject(bg);
    bg->AddComponent(new Sprite("Assets/fundo.png", bg_ptr));
    bg->AddComponent(new CameraFollower(bg_ptr));

    auto txt_start_go = new GameObject;
    auto txt_start_go_ptr = AddObject(txt_start_go);
    txt_start = new Text("Assets/Ubuntu-Regular.ttf", txt_start_go_ptr, 30, Text::SOLID, "Start", {0, 0, 0, 255});
    txt_start_go->AddComponent(txt_start);
    Rect &fontLuxuriaBox = txt_start_go->box;
    fontLuxuriaBox.x = GAME_WIDTH/2 - fontLuxuriaBox.w/2;
    fontLuxuriaBox.y = GAME_HEIGHT/2 - fontLuxuriaBox.h;
}

TitleState::~TitleState() {

}

void TitleState::LoadAssets() {

}

void TitleState::Update(float dt) {
    key_delay.Update(dt);
    InputManager &input = InputManager::GetInstance();
    quitRequested = input.QuitRequested();
    popRequested = input.KeyPress(ESCAPE_KEY);

    if (input.KeyPress(SDLK_SPACE) || input.KeyPress(SDLK_RETURN)) {
        txt_start->SetColor({255, 0, 0, 255});
        key_pressed = true;
        key_delay.Restart();
    }

    if(key_pressed && key_delay.Get()>0.3f){
        key_pressed = false;
        txt_start->SetColor({0, 0, 0, 255});
        // Game::GetInstance()->Push(new PreguicaState());
        Game::GetInstance()->Push(new LuxuriaState());
    }

    UpdateArray(dt);
}


void TitleState::Render() {
    RenderArray();
}

void TitleState::Start() {
    StartArray();
}

void TitleState::Pause() {

}

void TitleState::Resume() {
    Camera::GetInstance().pos = Vec2(0, 0);
}