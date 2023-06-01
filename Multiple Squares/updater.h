#pragma once
#include "Square.h"
#include <bitset>
#include "Networking.h"
#include "menu.h"
class updater {
public:
	updater(SDL_Window* win, SDL_Renderer* ren, uint_least8_t* gamestate, SDL_Event* ev);
	updater* handle_input(Square& sqr, double fps);
	updater* handle_border_collision(Square& sqr);
	updater* draw_sqr(Square& sqr);
	updater* draw_titles(title& title);
	updater* handle_host_b(host_b& host);
	updater* handle_join_b(join_b& join);
	updater* handle_tomenu_b(tomenu_b& gsw);
	updater* handle_lchost_b(lchost_b& lct);
	updater* handle_phost_b(phost_b& shs);
	updater* handle_hostinfo(hostinfo& hif);
	updater* handle_field(field& cfd, sockaddr_in& dest);
	updater* draw_enter2join(enter2join& e2j);
	updater* closefont(TTF_Font* font);
private:
	SDL_Window* win{};
	SDL_Renderer* ren{};
	SDL_Event* ev{};
	unsigned char* gamestate{};
	int_least32_t winsizex{};
	int_least32_t winsizey{};
};

