#pragma once

#define INCLUDE_SDL_TTF
#include "../../SDL_include.h"

#include <memory>
#include "../Component/Component.hpp"

class Text : public Component
{
public:
	enum TextStyle
	{
		SOLID,
		SHADED,
		BLENDED
	};
	
	Text(std::string fontFile, std::weak_ptr<GameObject> associated,  int fontSize, TextStyle style, std::string text, SDL_Color color, uint32_t wrap_size=200);
	~Text();

	void Render();
	void Render(int x, int y);
	void Update(float dt);

	bool Is(std::string type);
	void SetText(std::string text);
	void SetText(std::string_view text);
	void SetColor(SDL_Color color);
	void SetStyle(TextStyle style);
	void SetFontFile(std::string fontFile);
	void SetFontSize(int fontSize);
	void SetScale(float scaleX, float scaleY);

	bool show = true;

private:
	int width;
	int height;
	int wrap_size;
    Vec2 scale = {1,1};
	TTF_Font * font;
	SDL_Texture *texture;
	std::string text;
	TextStyle style;
	std::string fontFile;
	int fontSize;
	SDL_Color color;

	void RemakeTexture();
};