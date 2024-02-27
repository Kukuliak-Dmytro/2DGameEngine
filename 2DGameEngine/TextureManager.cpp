#include "TextureManager.h"
#include <string>
#include "FontManager.h"


 SDL_Texture* TextureManager::LoadTexture(const char* texture) {
	 SDL_Surface* tempSurface = IMG_Load(texture);
	 SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	 SDL_FreeSurface(tempSurface);

	 return tex;



}
 SDL_Texture* TextureManager::LoadFont(std::string fontPath, int fontSize, const char* fontText) {
	 TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
	 SDL_Color color = { 255,255,255,255 };
	 SDL_Surface* tempSurface = TTF_RenderText_Blended(font, fontText,color);
	 SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	 SDL_FreeSurface(tempSurface);

	 return tex;



 }
   void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle)
 {
	 
	 SDL_RenderCopyEx(Game::renderer, tex, &src, &dest,angle,NULL,flip);
 }
   