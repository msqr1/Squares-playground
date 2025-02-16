﻿#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <algorithm>
#include <string>
struct title {
	const char* ttd{ "Squares playground" };
	const char* jointtd{ "Enter Code:" };
	const char* hostttd{ "Select an option:" };
	const SDL_Color color = { 0,0,0 };
	TTF_Font* font{ TTF_OpenFont("tales.otf", 130) };
	TTF_Font* joinfont{ TTF_OpenFont("Calibri.ttf", 105) };
	TTF_Font* hostfont{ TTF_OpenFont("times.ttf", 105) };
};
struct host_b {
	const char* ttd{ "  Host Game  " };
	SDL_Color color = { 0,0,0 };
	const SDL_Color selectcolor{ 255,255,0 };
	const SDL_Color boxcolor{ 0,240,240 };
	SDL_Color storage{ this->color };
	TTF_Font* font = TTF_OpenFont("arcadeclassic.ttf", 70);
};
struct join_b {
	const char* ttd{ "  Join Game  " };
	SDL_Color color = { 0,0,0 };
	const SDL_Color selectcolor{ 255,255,0 };
	const SDL_Color boxcolor{ 0,240,240 };
	SDL_Color storage{ this->color };
	TTF_Font* font = TTF_OpenFont("arcadeclassic.ttf", 70);
};
struct tomenu_b {
	const char* ttd{ " << " };
	SDL_Color color = { 0,0,0 };
	const SDL_Color selectcolor{ 255,255,0 };
	const SDL_Color boxcolor{ 0,240,240 };
	SDL_Color storage{ this->color };
	TTF_Font* font = TTF_OpenFont("calibri.ttf", 40);
};
struct lchost_b {
	const char* ttd{ "   Localhost " };
	SDL_Color color = { 0,0,0 };
	SDL_Color selectcolor{ 255,255,0 };
	const SDL_Color boxcolor{ 0,240,240 };
	SDL_Color storage{ this->color };
	TTF_Font* font = TTF_OpenFont("arcadeclassic.ttf", 70);
};
struct phost_b {
	const char* ttd{ " Public host (coming soon) " };
	SDL_Color color = { 0,0,0 };
	SDL_Color selectcolor{ 255,255,0 };
	const SDL_Color boxcolor{ 0,240,240 };
	SDL_Color storage{ this->color };
	TTF_Font* font = TTF_OpenFont("arcadeclassic.ttf", 70);
};
struct hostinfo {
	const SDL_Color color = { 0,0,0 };
	std::string ttd{};
	std::string storage{};
	const SDL_Color boxcolor{ 192,192,192 };
	TTF_Font* font = TTF_OpenFont("calibri.ttf", 40);
};
struct field {
	std::string ttd{};
	const unsigned short borderthickness{ 15 };
	const SDL_Color textcolor{ 0,0,0 };
	TTF_Font* font{ TTF_OpenFont("times.ttf",60) };
};
struct enter2join {
	const char* ttd{ "Press \"Enter\" to join" };
	const SDL_Color color = { 0,0,0 };
	TTF_Font* font{ TTF_OpenFont("times.ttf", 105) };
};
struct authorize {
	const char* ttd{ "Authorization failed, please retry." };
	const SDL_Color color = { 0,0,0 };
	bool dflag : 1;
	TTF_Font* font{ TTF_OpenFont("times.ttf", 25) };
};
