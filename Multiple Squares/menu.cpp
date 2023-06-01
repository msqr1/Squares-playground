#include "menu.h"
//Game code (hostinfo)
void hostinfo::cpy2clip() {
	if (SDL_GetClipboardText() != this->storage) {
		SDL_SetClipboardText(this->storage.c_str());
	}
}
//Enter code (field)
field::field() {
	this->ttd.reserve(6);
}
void field::handle_del() {
	this->ttd.erase(this->ttd.end() - ((this->ttd.length() > 0) ? 1 : 0), this->ttd.end());
}
void field::handle_paste() {
	if (SDL_HasClipboardText()) {
		this->ttd += SDL_GetClipboardText();
		this->cut_6();
	}
}
void field::cut_6() {
	this->ttd = this->ttd.substr(0, 6);
}
