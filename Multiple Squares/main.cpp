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
	SDL_Renderer* ren{ SDL_CreateRenderer(win, -1,  SDL_RENDERER_ACCELERATED) };
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
	authorize auth{};
	auth.dflag = 0;
	Square tsqr{};
	TTF_Font* namefont{ TTF_OpenFont("times.ttf", 20) };
	Sqrc squares{};
	squares.reserve(max_player);
	//Networking
	sockaddr_in dest{};
	SOCKET tsock{};
	pollfd set{};
	//Timer for movement and other variables
	bool condition{ true };
	std::chrono::steady_clock::time_point later {};
	double deltat{};
	uint_least64_t p1{};
	while (loop) {
		deltat = (SDL_GetPerformanceCounter() - p1 + 0.0) / SDL_GetPerformanceFrequency();
		p1 = SDL_GetPerformanceCounter();
		//performance control
		switch (gamestate) {
		case GAMEPLAY:
			SDL_PollEvent(&ev);
			break;
		case PREJOIN:
		case JOINMENU:
			SDL_WaitEventTimeout(&ev, 50);
			break;
		default:
			SDL_WaitEvent(&ev);
			break;
		}
		if (SDL_GetWindowFlags(win) & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_HIDDEN)) {
			SDL_WaitEvent(&ev);
		}
		//main event handler
		switch (ev.type) {
		case SDL_QUIT: {
			lReq(dest, tsock, tsqr.index);
			shutdown(tsock, SD_BOTH);
			closesocket(tsock);
			loop = false;
		}
		}
		//gamestate
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
		SDL_RenderClear(ren);
		switch (gamestate) {
		case MAINMENU: {
			upr.draw_titles(tt)->handle_host_b(hb)->handle_join_b(jb, auth, cfd);		
			break;
		}
		case JOINMENU: {
			[&](){if (auth.dflag == 1) {
				if (condition) {
					later = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
					condition = false;
				}
				if (std::chrono::steady_clock::now() >= later) {
					auth.dflag = 0;
					condition = true;
					return;
				}
			}}();
			upr.draw_titles(tt)->handle_tomenu_b(tmb)->handle_field(cfd, dest, auth)
				->draw_enter2join(e2j)->draw_authorize(auth);
			break;
		}
		case GAMEPLAY: {
			set.events = POLLRDNORM | POLLWRNORM;
			if (WSAPoll(&set, 1, 0) == 1 && (set.revents == (POLLRDNORM | POLLWRNORM))) {
				processMsg(tsock, squares, tsqr, dest);	
			}
			set.events = POLLWRNORM;
			if (WSAPoll(&set, 1, 0) == 1 && set.revents == POLLWRNORM && std::chrono::steady_clock::now() >= later) {
				pUpdate(dest, tsock, tsqr);
				later = std::chrono::steady_clock::now() + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(0.1 - deltat));
			}
			//Handle this square
			upr.handle_input(tsqr, 1/deltat)->handle_border_collision(tsqr)->draw_sqr(tsqr, namefont, tsqr.index);
			//Other player square handle
			for (auto& i : squares) {
				upr.draw_sqr(*i.second, namefont, tsqr.index);
			}
			upr.handle_hostinfo(hif);
			break;
		}
		case HOSTMENU: {
			upr.handle_lchost_b(lct)->handle_tomenu_b(tmb)->handle_phost_b(phs)->draw_titles(tt);
			break;
		}
		case PREHOST: {
			int res{ prepareRoom(dest, tsock, tsqr) };
			if (res != 0) {
				std::cout << res;
				closesocket(tsock);
				gamestate = HOSTMENU;
				break;
			};
			hif.storage = std::to_string(dest.sin_port * 3);
			std::cout << "Real hosted port: " << ntohs(dest.sin_port);
			set.fd = tsock;
			set.events = POLLRDNORM | POLLWRNORM;
			gamestate = GAMEPLAY;
			break;
		}
		case PREJOIN: {
			int res{ joinRoom(dest, tsock, tsqr) };
			if (res != 0) {
				closesocket(tsock);
				auth.dflag = 1;
				std::cout << res;
				gamestate = JOINMENU;
				break;
			};
			iReq(dest, tsock, tsqr);
			if (processMsg(tsock, squares, tsqr, dest) == 0) {
				set.fd = tsock;
				hif.storage = std::to_string(dest.sin_port * 3);
				std::cout << "Real joined port: " << ntohs(dest.sin_port) << '\n';
				gamestate = GAMEPLAY;
			}
			else {
				closesocket(tsock);
				auth.dflag = 1;
				gamestate = JOINMENU;
			}
			break;
		}
		default: {
			std::cout << "Error: No such gamestate";
			loop = false;
			break;
		}
		}
		SDL_RenderPresent(ren);
	}
	upr.closefont(tt.font)->closefont(tt.joinfont)->closefont(tt.hostfont)
		->closefont(tmb.font)->closefont(hb.font)->closefont(jb.font)
		->closefont(hif.font)->closefont(phs.font)->closefont(lct.font);
	TTF_Quit();
	SDL_Quit();
}