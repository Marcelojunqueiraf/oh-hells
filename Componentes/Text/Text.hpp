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
	
	Text(std::string fontFile, std::weak_ptr<GameObject> associated,  int fontSize, TextStyle style, std::string text, SDL_Color color);
	~Text();

	void Render();
	void Update(float dt);

	bool Is(std::string type);
	void SetText(std::string text);
	void SetText(std::string_view text);
	void SetColor(SDL_Color color);
	void SetStyle(TextStyle style);
	void SetFontFile(std::string fontFile);
	void SetFontSize(int fontSize);

	bool show = true;

private:
	TTF_Font * font;
	SDL_Texture *texture;
	std::string text;
	TextStyle style;
	std::string fontFile;
	int fontSize;
	SDL_Color color;

	void RemakeTexture();
};