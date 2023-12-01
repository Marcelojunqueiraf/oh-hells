#include "Dialog.hpp"

static Sprite *last_character_animation = nullptr;

Dialog::Dialog(std::weak_ptr<GameObject> associated) 
: Component(associated)
{
    background = new Sprite("Assets/Menu_dialogo.png",associated);
    associated.lock()->AddComponent(background);
    background->show = false;
}

Dialog::~Dialog(){}

void Dialog::Hide(){
    background->show = false;
    if(last_character_animation)
        last_character_animation->show = false;
}

void Dialog::ShowDialog(Sprite * emotion, dialog_info dialog){
    background->show = true;

    last_character_animation = emotion;
    last_character_animation->show = true;

    character_name->SetText(dialog.character_name);
    character_name->show = true;

    character_message->SetText(dialog.character_msg);
    character_message->show = true;
}

bool Dialog::Is(std::string type){
    return type == "Dialog";
}

void Dialog::Update(float dt){
}

void Dialog::Render(){

}

void Dialog::Start(){
}