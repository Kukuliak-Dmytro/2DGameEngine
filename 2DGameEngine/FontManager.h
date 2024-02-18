#pragma once
#include <string>
#include "SDL_ttf.h"
#include "ECS.h"
#include <map>
std::map<std::string, TTF_Font*> fonts;

void addFont(std::string id, std::string path, int fontSize) {
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* GetFont(std::string id) { return fonts[id]; }