#include "State.hpp"
#include "../../Utils/Collision/Collision.cpp"

State::State() : popRequested(false), quitRequested(false), started(false) {}

State::~State() {
    for(auto & obj : objectArray) {
        obj.reset();
    }
    objectArray.clear();
}

std::weak_ptr<GameObject> State::AddObject(GameObject *go) {
	std::shared_ptr<GameObject> sh_go(go);
	objectArray.push_back(sh_go);
	if (started)
		sh_go->Start();

	return std::weak_ptr<GameObject>(sh_go);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject *go) {
    for (const auto &game_obj : objectArray)
	{
		if (game_obj.get() == go)
			return std::weak_ptr<GameObject>(game_obj);
	}
	return std::weak_ptr<GameObject>();
}

bool State::PopRequested() {
    return popRequested;
}

bool State::QuitRequested() {
    return quitRequested;
}

void State::StartArray() {
    for (size_t i = 0; i < objectArray.size(); i++)
		objectArray[i]->Start();
	started = true;
}

void State::UpdateArray(float dt) {
    for (size_t i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Update(dt);
	}

    for (size_t i = 0; i < objectArray.size(); i++) {
		if(objectArray[i]->IsDead()) {
			objectArray.erase(objectArray.begin()+i);
		}
	}
}

void State::RenderArray() {
    for(size_t i = 0; i < objectArray.size(); i++){
        objectArray[i].get()->Render();
    }
}

void State::VerifyCollision() {
    for (size_t i = 0; i < objectArray.size(); i++) {
        for(size_t j = i+1; j < objectArray.size(); j++){
            auto &objA = objectArray[i];
            auto &objB = objectArray[j];

            Collider * colliderA = (Collider *)objA->GetComponent("Collider").lock().get();
            Collider * colliderB = (Collider *)objB->GetComponent("Collider").lock().get();
            
            if(colliderA && colliderB){
                auto boxA = colliderA->box;
                auto boxB = colliderB->box;
                if (Collision::IsColliding(boxA, boxB, objA->angle, objB->angle)) {
                    objA->NotifyCollision(objB);
                    objB->NotifyCollision(objA);
                }
            }
        }
    }
}