#include "Text.hpp"
#include "../../Utils/Resources/Resources.hpp"
#include "../../Game/Game.hpp"
#include "../../Camera/Camera.hpp"

Text::Text(std::string fontFile, std::weak_ptr<GameObject> associated, int fontSize, TextStyle style, std::string text, SDL_Color color) : Component(associated) {
	texture = nullptr;
	this->text = text;
	this->style = style;
	this->fontFile = fontFile;
	this->fontSize = fontSize;
	this->color = color;
	font = Resources::GetFont(fontFile, fontSize);

	RemakeTexture();
}

Text::~Text() {
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
}

void Text::Update(float dt) {

}

void Text::Render() {
	if (texture != nullptr) {
		SDL_Rect clipRect;
		clipRect.x = 0;
		clipRect.y = 0;
		clipRect.w = associated.lock()->box.w;
		clipRect.h = associated.lock()->box.h;

		SDL_Rect dstRect;
		dstRect.x = associated.lock()->box.x + Camera::GetInstance().pos.x;
		dstRect.y = associated.lock()->box.y + Camera::GetInstance().pos.y;
		dstRect.w = associated.lock()->box.w;
		dstRect.h = associated.lock()->box.h;

		SDL_RenderCopyEx(Game::GetInstance()->GetRenderer().lock().get(), texture, &clipRect, &dstRect, (180/M_PI)*associated.lock()->angle, nullptr, SDL_FLIP_NONE);
	}
}

bool Text::Is(std::string type) {
	return type == "Text";
}

void Text::SetText(std::string text) {
	this->text = text;
	RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
	this->color = color;
	RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
	this->style = style;
	RemakeTexture();
}

void Text::SetFontFile(std::string fontFile) {
	this->fontFile = fontFile;
	RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
	this->fontSize = fontSize;
	RemakeTexture();
}

void Text::RemakeTexture() {
	if (texture)
		SDL_DestroyTexture(texture);

	font = Resources::GetFont(fontFile, fontSize);
	SDL_Surface* surf = nullptr;

	switch (style) {
		case SOLID:
			surf = TTF_RenderText_Solid(font, text.c_str(), color);
			break;
		case SHADED:
			surf = TTF_RenderText_Shaded(font, text.c_str(), color, {0, 0, 0, 255});
			break;
		case BLENDED:
			surf = TTF_RenderText_Blended(font, text.c_str(), color);
			break;
		default:
			break;
	}

	if (surf == nullptr) {
		printf("Error loading Texture: %s", SDL_GetError());
		exit(1);
	}

	texture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer().lock().get(), surf);

	if (texture == nullptr) {
		printf("Error loading Texture: %s", SDL_GetError());
		std::abort();
	}

	associated.lock()->box.h = surf->h;
	associated.lock()->box.w = surf->w;

	SDL_FreeSurface(surf);
}