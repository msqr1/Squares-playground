#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <WinInet.h>
#include <iphlpapi.h>
#include <set>
#include <numeric>
#include <random>
#include <variant>
#include "Square.h"
#include "Constants.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib, "wininet.lib") 
struct Position_Update {
	uint_least16_t x{};
	uint_least16_t y{};
	uint_least8_t index{};
	Position_Update(Square& tsqr) : x(static_cast<uint_least16_t>(std::lround(tsqr.posx))), y(static_cast<uint_least16_t>(std::lround(tsqr.posy))), index(tsqr.index) {};
	void operator=(Square& sqr) {
		this->x = static_cast<uint_least16_t>(std::lround(sqr.posx));
		this->y = static_cast<uint_least16_t>(std::lround(sqr.posy));
	}
	bool operator==(Square& sqr) {
		return this->x == static_cast<uint_least16_t>(std::lround(sqr.posx)) && this->y == static_cast<uint_least16_t>(std::lround(sqr.posy));;
	}
	Position_Update() {};
};
struct Info_Request {
	char name[16]{};
	uint_least8_t r{};
	uint_least8_t g{};
	uint_least8_t b{};
	Info_Request(Square& tsqr) : r(tsqr.r), g(tsqr.g), b(tsqr.b) {
		memcpy_s(this->name, 16, tsqr.name, 16);
	}
};
struct Info_Reply {
	Square tsqr{};
	uint_least8_t nIndex{};
	Info_Reply(Square& tsqr, uint_least8_t nIndex) : tsqr(tsqr), nIndex(nIndex) {}
};
typedef std::variant<Position_Update, Info_Request, Info_Reply, uint_least8_t> msg;
unsigned long long rng(unsigned long long min, unsigned long long max);
int joinRoom(sockaddr_in& sin, SOCKET& s, Square& tsqr);
int prepareRoom(sockaddr_in& sin, SOCKET& s, Square& tsqr);
int pUpdate(sockaddr_in& sin, SOCKET& s, Square& tsqr);
int iReq(sockaddr_in& sin, SOCKET& s, Square& tsqr);
int iRep(sockaddr_in& sin, SOCKET& s, Square& tsqr, uint_least8_t newindex);
int lReq(sockaddr_in& sin, SOCKET& s, uint_least8_t thisindex);
int processMsg(SOCKET& s, Sqrc& c, Square& tsqr, sockaddr_in& sin);
