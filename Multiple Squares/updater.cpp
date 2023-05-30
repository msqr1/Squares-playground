#include "updater.h"
#include "Constants.h"
updater::updater(SDL_Window* win, SDL_Renderer* ren, uint_least8_t* gamestate, SDL_Event* ev)
	: win(win), ren(ren), gamestate(gamestate), ev(ev) {
	SDL_GetWindowSize(win, &this->winsizex, &this->winsizey);
}
updater* updater::draw_sqr(Square& sqr) {
	SDL_Rect todraw{ std::lroundl(sqr.posx), std::lround(sqr.posy), P_SIZE, P_SIZE };
	SDL_SetRenderDrawColor(this->ren, sqr.r, sqr.g, sqr.b, 0);
	SDL_RenderDrawRect(this->ren, &todraw);
	SDL_RenderFillRect(this->ren, &todraw);
	SDL_SetRenderDrawColor(this->ren, 255, 255, 255, 0);
	return this;
}
updater* updater::handle_input(Square& sqr, double fps) {
	static bool w;
	static bool s;
	static bool a;
	static bool d;
	if (ev->type == SDL_KEYDOWN && ev->key.repeat == 0) {
		switch (ev->key.keysym.scancode) {
		case SDL_SCANCODE_W: w = true; break;
		case SDL_SCANCODE_S: s = true; break;
		case SDL_SCANCODE_A: a = true; break;
		case SDL_SCANCODE_D: d = true; break;
		}
	}
	if (ev->type == SDL_KEYUP) {
		switch (ev->key.keysym.scancode) {
		case SDL_SCANCODE_W: w = false; break;
		case SDL_SCANCODE_S: s = false; break;
		case SDL_SCANCODE_A: a = false; break;
		case SDL_SCANCODE_D: d = false; break;
		}
	}
	double xspeed{ P_SPEED / fps * (a ^ d ? a ? -1 : 1 : 0) };
	double yspeed{ P_SPEED / fps * (w ^ s ? w ? -1 : 1 : 0) };
	if (xspeed != 0 && yspeed != 0) {
		xspeed *= .7071;
		yspeed *= .7071;
	}
	sqr.posx += xspeed;
	sqr.posy += yspeed;
	return this;
}
updater* updater::handle_border_collision(Square& sqr) {
	if (sqr.posx <= 0) {
		sqr.posx = 0;
	}
	if (sqr.posy <= 0) {
		sqr.posy = 0;
	}
	if (sqr.posx + P_SIZE >= this->winsizex) {
		sqr.posx = this->winsizex - P_SIZE;
	}
	if (sqr.posy + P_SIZE >= this->winsizey) {
		sqr.posy = this->winsizey - P_SIZE;
	}
	return this;
}
updater* updater::draw_titles(title& title) {
	SDL_Surface* surface{};
	switch (*this->gamestate) {
	case 0:
		surface = TTF_RenderUTF8_Blended(title.font, title.ttd, title.color);
		break;
	case 1:
		surface = TTF_RenderUTF8_Blended(title.joinfont, title.jointtd, title.color);
		break;
	case 3:
		surface = TTF_RenderUTF8_Blended(title.hostfont, title.hostttd, title.color);
	}
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	if (*this->gamestate == 0) {
		texW += static_cast<int_least32_t>(texW * 0.3); //Stretch the text width by 1.3 times
	}
	SDL_Rect dstrect = { static_cast<int_least32_t>(winsizex / 2 - texW / 2),static_cast<int_least32_t>(winsizey / 4 - texH / 1.5), texW, texH };
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_host_b(host_b& host) {
	SDL_Surface* surface{ TTF_RenderUTF8_Shaded(host.font,host.ttd,host.color,host.boxcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { static_cast<int_least32_t>(winsizex / 2 - texW / 2),static_cast<int_least32_t>(winsizey / 2 - texH / 2), texW, texH };
	int_least32_t x{}, y{};
	uint_least32_t buttons{};
	SDL_PumpEvents();
	buttons = SDL_GetMouseState(&x, &y);
	if (y > static_cast<int_least32_t>(winsizey / 2.0f - texH / 1.5) && y < static_cast<int_least32_t>(winsizey / 2 - texH / 2) + texH) {
		if (x > static_cast<int_least32_t>(winsizex / 2 - texW / 2) && x < static_cast<int_least32_t>(winsizex / 2 - texW / 2) + texW) {
			host.color = host.selectcolor;
			if ((buttons & SDL_BUTTON_LMASK) != 0) {
				*this->gamestate = 3;
				SDL_Delay(110);
			}
		}
		else { host.color = host.storage; }
	}
	else { host.color = host.storage; }
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_join_b(join_b& join) {
	SDL_Surface* surface{ TTF_RenderUTF8_Shaded(join.font, join.ttd, join.color, join.boxcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { static_cast<int_least32_t>(winsizex / 2 - texW / 2),static_cast<int_least32_t>(winsizey / 1.4), texW, texH };
	int_least32_t x{}, y{};
	uint_least32_t buttons{};
	SDL_PumpEvents();
	buttons = SDL_GetMouseState(&x, &y);
	if (y > static_cast<int_least32_t>(winsizey / 1.4) && y < static_cast<int_least32_t>(winsizey / 1.4) + texH) {
		if (x > static_cast<int_least32_t>(winsizex / 2 - texW / 2) && x < static_cast<int_least32_t>(winsizex / 2 - texW / 2) + texW) {
			join.color = join.selectcolor;
			if ((buttons & SDL_BUTTON_LMASK) != 0) {
				*this->gamestate = JOINMENU;
			}
		}
		else { join.color = join.storage; }

	}
	else { join.color = join.storage; }
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_tomenu_b(tomenu_b& tmn) {
	SDL_Surface* surface{ TTF_RenderUTF8_Shaded(tmn.font,tmn.ttd,tmn.color,tmn.boxcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { 10,10, texW, texH };
	int_least32_t x{}, y{};
	Uint32 buttons{};
	SDL_PumpEvents();
	buttons = SDL_GetMouseState(&x, &y);
	if (y > 10 && y < 10 + texH) {
		if (x > 10 && x < 10 + texW) {
			tmn.color = tmn.selectcolor;
			if ((buttons & SDL_BUTTON_LMASK) != 0) {
				*this->gamestate = MAINMENU;
			}
		}
		else { tmn.color = tmn.storage; }
	}
	else { tmn.color = tmn.storage; }
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_lchost_b(lchost_b& lct) {
	SDL_Surface* surface{ TTF_RenderUTF8_Shaded(lct.font,lct.ttd,lct.color,lct.boxcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { static_cast<int_least32_t>(winsizex / 2 - texW / 2),static_cast<int_least32_t>(winsizey / 2 - texH / 2), texW, texH };
	int_least32_t x{}, y{};
	uint_least32_t buttons{};
	SDL_PumpEvents();
	buttons = SDL_GetMouseState(&x, &y);
	if (y > static_cast<int_least32_t>(winsizey / 2.0f - texH / 1.5) && y < static_cast<int_least32_t>(winsizey / 2 - texH / 2) + texH) {
		if (x > static_cast<int_least32_t>(winsizex / 2 - texW / 2) && x < static_cast<int_least32_t>(winsizex / 2 - texW / 2) + texW) {
			lct.color = lct.selectcolor;
			if ((buttons & SDL_BUTTON_LMASK) != 0) {
				*this->gamestate = 4;
			}
		}
		else { lct.color = lct.storage; }
	}
	else { lct.color = lct.storage; }
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_phost_b(phost_b& phs) {
	SDL_Surface* surface{ TTF_RenderUTF8_Shaded(phs.font,phs.ttd,phs.color,phs.boxcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { static_cast<int_least32_t>(winsizex / 2 - texW / 2),static_cast<int_least32_t>(winsizey / 1.4), texW, texH };
	int_least32_t x{}, y{};
	uint_least32_t buttons{};
	SDL_PumpEvents();
	buttons = SDL_GetMouseState(&x, &y);
	if (y > static_cast<int_least32_t>(winsizey / 1.4) && y < static_cast<int_least32_t>(winsizey / 1.4) + texH) {
		if (x > static_cast<int_least32_t>(winsizex / 2 - texW / 2) && x < static_cast<int_least32_t>(winsizex / 2 - texW / 2) + texW) {
			phs.color = phs.selectcolor;
			if ((buttons & SDL_BUTTON_LMASK) != 0) {
				*this->gamestate = 2;
			}
		}
		else { phs.color = phs.storage; }
	}
	else { phs.color = phs.storage; }
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_hostinfo(hostinfo& hif) {
	SDL_Surface* surface{ TTF_RenderUTF8_Shaded(hif.font,(' ' + hif.ttd + ' ').c_str(),hif.color,hif.boxcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { this->winsizex - texW,this->winsizey - texH, texW, texH };
	int_least32_t x{}, y{};
	SDL_PumpEvents();
	uint_least32_t buttons{ SDL_GetMouseState(&x, &y) };
	if ((y > static_cast<int_least32_t>(winsizey - texH) && y < this->winsizey)
		&& (x > static_cast<int_least32_t>(winsizex - texW) && x < this->winsizex)) {
		hif.ttd = "   Copy!   ";
		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			hif.cpy2clip();
		}
	}
	else { hif.ttd = hif.storage; }
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::handle_field(field& cfd, sockaddr_in& dest) {
	SDL_StartTextInput();
	SDL_PumpEvents();
	switch (this->ev->type) {
	case SDL_TEXTINPUT:
		if (cfd.ttd.length() > 5) { SDL_StopTextInput(); break; }
		else { SDL_StartTextInput(); cfd.ttd += this->ev->text.text; break; }
	case SDL_KEYDOWN:
		switch (this->ev->key.keysym.scancode) {
		case SDL_SCANCODE_BACKSPACE:
			cfd.handle_del();
			break;
		case SDL_SCANCODE_V:
			if (SDL_GetModState() & KMOD_CTRL) {
				cfd.handle_paste();
			}
			break;
		case SDL_SCANCODE_RETURN:
		case SDL_SCANCODE_KP_ENTER:
			dest.sin_port = static_cast<USHORT>(std::stoi(cfd.ttd) / 3);
			*this->gamestate = PREJOIN;
			break;
		}
	}
	SDL_Surface* surface{ TTF_RenderUTF8_Blended(cfd.font,cfd.ttd.c_str(),cfd.textcolor) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	int_least32_t texX{ static_cast<int_least32_t>(this->winsizex / 2 - texW / 2) };
	int_least32_t texY{ static_cast<int_least32_t>(this->winsizey / 2 - texH / 2) };
	int_least32_t rectX{ 580 }, rectY{ 361 }, rectW{ 375 }, rectH{ 73 };
	SDL_Rect fgrect = { rectX,rectY,rectW,rectH };
	SDL_Rect bgrect = { rectX - cfd.borderthickness / 2,rectY - cfd.borderthickness / 2,rectW + cfd.borderthickness,rectH + cfd.borderthickness };
	SDL_Rect dstrect = { texX,texY,texW,texH };
	SDL_SetRenderDrawColor(this->ren, 0, 0, 0, 0);
	SDL_RenderDrawRect(this->ren, &bgrect);
	SDL_RenderFillRect(this->ren, &bgrect);
	SDL_SetRenderDrawColor(this->ren, 255, 255, 255, 0);
	SDL_RenderDrawRect(this->ren, &fgrect);
	SDL_RenderFillRect(this->ren, &fgrect);
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::draw_enter2join(enter2join& e2j) {
	SDL_Surface* surface{ TTF_RenderUTF8_Blended(e2j.font,e2j.ttd,e2j.color) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(this->ren,surface) };
	int_least32_t texW{}, texH{};
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect dstrect = { static_cast<int_least32_t>(winsizex / 2 - texW / 2),static_cast<int_least32_t>(winsizey / 2 + texH * 1.5), texW, texH };
	SDL_RenderCopy(this->ren, texture, nullptr, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return this;
}
updater* updater::closefont(TTF_Font* font) {
	TTF_CloseFont(font);
	return this;
}