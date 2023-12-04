#include "Text.hpp"
#include "../../Utils/Resources/Resources.hpp"
#include "../../Game/Game.hpp"
#include "../../Camera/Camera.hpp"

Text::Text(std::string fontFile, std::weak_ptr<GameObject> associated, int fontSize, TextStyle style, std::string text, SDL_Color color, uint32_t wrap_size)
		: Component(associated), wrap_size(wrap_size)
{
	texture = nullptr;
	this->text = text;
	this->style = style;
	this->fontFile = fontFile;
	this->fontSize = fontSize;
	this->color = color;
	font = Resources::GetFont(fontFile, fontSize);

	RemakeTexture();
}

Text::~Text()
{
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
}

void Text::SetScale(float scaleX, float scaleY)
{
	this->scale.x = scaleX;
	this->scale.y = scaleY;
	//   this->associated.lock()->box.w = this->width * scaleX;
	//   this->associated.lock()->box.h = this->height * scaleY;
}

void Text::Update(float dt)
{
}

void Text::Render(int x, int y)
{
	if (show)
		if (texture != nullptr)
		{

			auto assoc = associated.lock();

			SDL_Rect clipRect;
			clipRect.x = 0;
			clipRect.y = 0;
			clipRect.w = width;
			clipRect.h = height;

			SDL_Rect dstRect;
			Camera &camera = Camera::GetInstance();
			dstRect.x = assoc->box.x + x - camera.pos.x;
			dstRect.y = assoc->box.y + y - camera.pos.y;
			dstRect.w = clipRect.w * scale.x;
			dstRect.h = clipRect.h * scale.y;

			SDL_RenderCopyEx(Game::GetInstance()->GetRenderer().lock().get(), texture, &clipRect, &dstRect, (180 / M_PI) * associated.lock()->angle, nullptr, SDL_FLIP_NONE);
		}
}

void Text::Render()
{
	Render(0, 0);
}

bool Text::Is(std::string type)
{
	return type == "Text";
}

void Text::SetText(std::string text)
{
	this->text = text;
	RemakeTexture();
}

void Text::SetText(std::string_view text)
{
	this->text = text;
	RemakeTexture();
}

void Text::SetColor(SDL_Color color)
{
	this->color = color;
	RemakeTexture();
}

void Text::SetStyle(TextStyle style)
{
	this->style = style;
	RemakeTexture();
}

void Text::SetFontFile(std::string fontFile)
{
	this->fontFile = fontFile;
	RemakeTexture();
}

void Text::SetFontSize(int fontSize)
{
	this->fontSize = fontSize;
	RemakeTexture();
}

void Text::RemakeTexture()
{
	if (texture)
		SDL_DestroyTexture(texture);

	font = Resources::GetFont(fontFile, fontSize);
	SDL_Surface *surf = nullptr;

	// switch (style)
	// {
	// case SOLID:
	// 	surf = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), color, wrap_size);
	// 	break;
	// case SHADED:
	// 	surf = TTF_RenderUTF8_Shaded_Wrapped(font, text.c_str(), color, {0, 0, 0, 255}, wrap_size);
	// 	break;
	// case BLENDED:
	// 	surf = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrap_size);
	// 	break;
	// default:
	// 	break;
	// }
	surf = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrap_size);

	if (surf == nullptr)
	{
		printf("Error loading Texture: %s", SDL_GetError());
		exit(1);
	}

	texture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer().lock().get(), surf);

	if (texture == nullptr)
	{
		printf("Error loading Texture: %s", SDL_GetError());
		std::abort();
	}
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	associated.lock()->box.h = surf->h;
	associated.lock()->box.w = surf->w;

	SDL_FreeSurface(surf);
}