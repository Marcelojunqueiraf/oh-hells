#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "../../SDL_include.h"

#include <memory>
#include <vector>

#include "../../Componentes/Sprite/Sprite.hpp"
#include "../../Music/Music.hpp"
#include "../../GameObject/GameObject.hpp"
#include "../../Utils/Vec2/Vec2.hpp"
#include "../../Utils/Rect/Rect.hpp"
#include "../../Componentes/Collider/Collider.hpp"

class State {
public:

	State();
	~State();

	virtual void LoadAssets() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	virtual void Start() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	std::weak_ptr<GameObject> AddObject(GameObject* go);
	std::weak_ptr<GameObject> GetObjectPtr(GameObject* go);

	bool QuitRequested();
	bool PopRequested();


protected:
    void StartArray();
    virtual void UpdateArray(float dt);
    virtual void RenderArray();
	virtual void VerifyCollision();

    bool popRequested;
    bool quitRequested;
    bool started;

	std::vector<std::shared_ptr<GameObject>> objectArray;

};