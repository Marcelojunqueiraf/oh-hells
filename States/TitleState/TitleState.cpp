#include "TitleState.hpp"
#include "../LuxuriaState/LuxuriaState.hpp"
#include "../../Game/Game.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Componentes/Text/Text.hpp"
#include "../../Componentes/Sprite/Sprite.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"

TitleState::TitleState() {
    auto bg = new GameObject;
    auto bg_ptr = AddObject(bg);
    bg->AddComponent(new Sprite("Assets/fundo.png", bg_ptr));
    bg->AddComponent(new CameraFollower(bg_ptr));

    auto text_go = new GameObject;
    auto text_go_ptr = AddObject(text_go);
    Text *text = new Text("Assets/Ubuntu-Regular.ttf", text_go_ptr, 30, Text::SOLID, "Pressione ESPACO para jogar...", {0, 0, 0, 255});
    text_go->AddComponent(text);
    Rect &fontBox = text_go->box;
    fontBox.x = GAME_WIDTH/2 - fontBox.w/2;
    fontBox.y = GAME_HEIGHT - fontBox.h;
}

TitleState::~TitleState() {

}

void TitleState::LoadAssets() {

}

void TitleState::Update(float dt) {
    auto inputManager = InputManager::GetInstance();

    UpdateArray(dt);

    quitRequested = inputManager.QuitRequested() || inputManager.KeyPress(ESCAPE_KEY);
    if (inputManager.KeyPress(SDLK_SPACE)) {
        Game::GetInstance()->Push(new LuxuriaState());
    }
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