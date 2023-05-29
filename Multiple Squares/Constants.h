#pragma once
#include <string>
constexpr double P_SPEED{ 100 }; //Speed in pixel per second
constexpr int P_SIZE{ 50 }; //pixels
const std::string checkarray{ "$BNTu2SnlF)jV_-mEP7=y+0W3bax@OMYAK?hU1L&4G^~JvqoZwgf{H!8z<`psDRIdk|CteX(#6ir>Q*59c};%" };
const std::string padarray{ "\"\',./[]\\" };
constexpr char no{ 0 };
constexpr char yes{ 1 };
constexpr const char def_addr[14]{ "224.3.255.250" };
constexpr uint_least8_t max_player{ 10 };
enum state { MAINMENU, JOINMENU, GAMEPLAY, HOSTMENU, PREHOST, PREJOIN };
