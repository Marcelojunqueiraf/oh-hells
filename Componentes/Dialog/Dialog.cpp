#include "Dialog.hpp"
#include "../CameraFollower/CameraFollower.hpp"

Dialog::Dialog(std::weak_ptr<GameObject> associated) 
: Component(associated)
{

    background = new Sprite("Assets/Menu_dialogo.png",associated);
    background->SetScaleX((float)GAME_WIDTH/background->GetWidth(), (float)GAME_HEIGHT/background->GetHeight());
    associated.lock()->AddComponent(background);
    associated.lock()->AddComponent(new CameraFollower(associated));
    background->show = false;
    
    character_name = new Text("Assets/Ubuntu-Regular.ttf", associated, 28, Text::BLENDED, "Chr name", {255, 255, 255, 255});
    character_message = new Text("Assets/Ubuntu-Regular.ttf", associated, 28, Text::BLENDED, "Chr message", {255, 255, 255, 255}, 500);
    character_name->show = false;
    character_message->show = false;
}

Dialog::~Dialog(){}

void Dialog::Hide(){
    background->show = false;
    if(last_character_animation)
        last_character_animation->show = false;
    
    character_name->show = false;
    character_message->show = false;
}

void Dialog::ShowDialog(Sprite * emotion, std::string chr_name, std::string chr_msg, float timeout){
    background->show = true;
    dialog_timeout1 = timeout;

    if(last_character_animation)
        last_character_animation->show = false; 

    last_character_animation = emotion;
    last_character_animation->show = true;

    character_name->SetText(std::move(chr_name));
    character_name->show = true;

    character_message->SetText(std::move(chr_msg));
    character_message->show = true;

    dialog_showing1 = true;
    dialog_timer1.Restart();
}

bool Dialog::Is(std::string type){
    return type == "Dialog";
}

void Dialog::Update(float dt){
    dialog_timer1.Update(dt);
    if(dialog_showing1 && dialog_timer1.Get() > dialog_timeout1){
        dialog_showing1 = false;
        background->show = false;
    }
}

void Dialog::Render(){
    if(dialog_showing1){
        if(last_character_animation)
            last_character_animation->Render();
        character_name->Render(390,380);
        character_message->Render(390,430);
    }
}

void Dialog::Start(){
}