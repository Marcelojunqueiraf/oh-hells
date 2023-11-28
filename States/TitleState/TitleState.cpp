#include "TitleState.hpp"
#include "../LuxuriaState/LuxuriaState.hpp"
#include "../PreguicaState/PreguicaState.hpp"
#include "../../Game/Game.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Componentes/Sprite/Sprite.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Componentes/CameraFollower/CameraFollower.hpp"

static Text * last_text = nullptr;
TitleState::TitleState() {
    auto bg = new GameObject;
    auto bg_ptr = AddObject(bg);
    bg->AddComponent(new Sprite("Assets/fundo.png", bg_ptr));
    bg->AddComponent(new CameraFollower(bg_ptr));

    auto txt_luxuria_option_go = new GameObject;
    auto txt_luxuria_option_go_ptr = AddObject(txt_luxuria_option_go);
    txt_luxuria_option = new Text("Assets/Ubuntu-Regular.ttf", txt_luxuria_option_go_ptr, 30, Text::BLENDED, "Fase 1", {255, 0, 0, 255});
    txt_luxuria_option_go->AddComponent(txt_luxuria_option);
    Rect &fontLuxuriaBox = txt_luxuria_option_go->box;
    fontLuxuriaBox.x = GAME_WIDTH/2 - fontLuxuriaBox.w/2;
    fontLuxuriaBox.y = GAME_HEIGHT/2 + fontLuxuriaBox.h;

    auto txt_preguica_go = new GameObject;
    auto txt_preguica_go_ptr = AddObject(txt_preguica_go);
    txt_preguica_option = new Text("Assets/Ubuntu-Regular.ttf", txt_preguica_go_ptr, 30, Text::BLENDED, "Fase 2", {0, 0, 0, 255});
    txt_preguica_go->AddComponent(txt_preguica_option);
    Rect &fontPreguicaBox = txt_preguica_go->box;
    fontPreguicaBox.x = GAME_WIDTH/2 - fontPreguicaBox.w/2;
    fontPreguicaBox.y = GAME_HEIGHT/2 - fontPreguicaBox.h;

    last_text = txt_luxuria_option;
}

TitleState::~TitleState() {

}

void TitleState::LoadAssets() {

}

void TitleState::Update(float dt) {
    auto inputManager = InputManager::GetInstance();

    UpdateArray(dt);

    key_delay.Update(dt);


    InputManager &input = InputManager::GetInstance();

    bool up = input.IsKeyDown(SDLK_w)   || input.IsKeyDown(UP_ARROW_KEY);
    bool down = input.IsKeyDown(SDLK_s) || input.IsKeyDown(DOWN_ARROW_KEY);
    bool left = input.IsKeyDown(SDLK_a) || input.IsKeyDown(LEFT_ARROW_KEY);
    bool right = input.IsKeyDown(SDLK_d)|| input.IsKeyDown(RIGHT_ARROW_KEY);

    quitRequested = inputManager.QuitRequested() || inputManager.KeyPress(ESCAPE_KEY);

    if(key_delay.Get() > 0.3f){
        if (inputManager.KeyPress(SDLK_SPACE)) {
            StartLevel();
        }else if(up){
            menu_option--;
        }else if(down){
            menu_option++;
        }

        if(menu_option == MENU_SIZE){
            menu_option = 0;
        }else if(menu_option < 0){
            menu_option = MENU_SIZE;
        }

        if(up || down){

            last_text->SetColor({0, 0, 0, 255});
            switch (menu_option)
            {
            case 0:
                last_text = txt_luxuria_option;
                break;
            case 1:
                last_text = txt_preguica_option;
            default:
                break;
            }
            last_text->SetColor({255, 0, 0, 255});
        }
    }
    
    

    
}

void TitleState::StartLevel(){
    switch (menu_option)
    {
    case 0:
        Game::GetInstance()->Push(new LuxuriaState());
        break;
    case 1:
        Game::GetInstance()->Push(new PreguicaState());
    default:
        break;
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