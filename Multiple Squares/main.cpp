#pragma once
#include "Square.h"
#include "updater.h"
#include "menu.h"
#include <iostream>
#undef main

int_least32_t main() {
	std::ios_base::sync_with_stdio(false);
	//Init part
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	WSADATA data{};
	WSAStartup(MAKEWORD(2, 2), &data);
	//Window & Renderer creation
	SDL_DisplayMode dm{};
	SDL_GetDesktopDisplayMode(0, &dm);
	int_least32_t width{ static_cast<int_least32_t> (dm.w / 2) };
	int_least32_t height{ static_cast<int_least32_t>(dm.h / 2) };
	SDL_Window* win{ SDL_CreateWindow("Single/Multiple square(s)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED) };
	SDL_Renderer* ren {SDL_CreateRenderer(win, -1,  SDL_RENDERER_ACCELERATED)};
	//Event List
	SDL_Event ev{};
	//Main loop & events
	bool loop{ true };
	unsigned char gamestate{ 0 };
	// Game mechanic part
	updater upr{ win,ren,&gamestate,&ev };
	title tt{};
	host_b hb{};
	join_b jb{};
	tomenu_b tmb{};
	hostinfo hif{};
	field cfd{};
	enter2join e2j{};
	lchost_b lct{};
	phost_b phs{};
	//Networking
	sockaddr_in dest{};
	SOCKET tsock{};
	Square tsqr{};
	Sqrc squares{};
	squares.reserve(10);
	//Time based movement & FPS limiter
	double fps{};
	uint_least64_t p1{};
	while (loop) {
		fps = 1 / ((SDL_GetPerformanceCounter() - p1 + 0.0) / SDL_GetPerformanceFrequency());
		p1 = SDL_GetPerformanceCounter();
		SDL_PollEvent(&ev);
		switch (ev.type) {
		case SDL_QUIT: {
			loop = false;
		}
		}
		//gamestate
		switch (gamestate) {
		case MAINMENU: { 
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
			SDL_RenderClear(ren);
			upr.draw_titles(tt)->handle_host_b(hb)->handle_join_b(jb);
			SDL_RenderPresent(ren);
			break;
		} 
		case JOINMENU: {  
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
			SDL_RenderClear(ren);
			upr.draw_titles(tt)->handle_tomenu_b(tmb)->handle_field(cfd, dest)->draw_enter2join(e2j);
			SDL_RenderPresent(ren);
			break;
		}
		case GAMEPLAY: { 
			SDL_RenderClear(ren);
			//Handle this square
			upr.handle_input(tsqr, fps)->handle_border_collision(tsqr)->draw_sqr(tsqr);
			//Other player square handle
			for (auto& i : squares) {
				upr.draw_sqr(*i.second);
			}
			upr.handle_hostinfo(hif);
			SDL_RenderPresent(ren);
			break;
		}
		case HOSTMENU: { 
			SDL_RenderClear(ren);
			upr.handle_lchost_b(lct)->handle_tomenu_b(tmb)->handle_phost_b(phs)->draw_titles(tt);
			SDL_RenderPresent(ren);
			break;
		}
		case PREHOST: { 
			prepareRoom(dest, tsock, tsqr);
			hif.storage = std::to_string(dest.sin_port*3);
			gamestate = GAMEPLAY;
			break;
		}
		case PREJOIN: { 
			joinRoom(dest, tsock, tsqr);
			iReq(dest, tsock, tsqr);
			gamestate = GAMEPLAY;
			break;
		}
		default: {
			std::cout <<  "Error: No such gamestate";
			loop = false;
			break;
		}
		}
	}
	closesocket(tsock);
	upr.closefont(tt.font)->closefont(tt.joinfont)->closefont(tt.hostfont)
		->closefont(tmb.font)->closefont(hb.font)->closefont(jb.font)
		->closefont(hif.font)->closefont(phs.font)->closefont(lct.font);
	TTF_Quit();
	SDL_Quit();
}