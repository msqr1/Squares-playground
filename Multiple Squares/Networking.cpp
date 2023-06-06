#pragma once
#include "Networking.h"
unsigned long long rng(unsigned long long min, unsigned long long max) {
	std::mt19937 generator((unsigned int)time(0));
	std::uniform_real_distribution<long double> maprange(static_cast<long double>(min), static_cast<long double>(max));
	return static_cast<unsigned long long>(maprange(generator));
}
int joinRoom(sockaddr_in& sin, SOCKET& s, Square& tsqr) {
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, def_addr, &sin.sin_addr);
	char hostname[256]{}; //Is actually DNS hostname, can't be playername because it has 255 characters limit
	char playername[16]{}; //Is actually computername because it has 15 characters limit
	DWORD playernamesize{ sizeof(playername) };
	if (GetComputerNameA(playername, &playernamesize) == 0) {
		return GetLastError();
	};
	if (gethostname(hostname, sizeof(hostname)) != 0) {
		return WSAGetLastError();
	}
	addrinfo* result{}, hints{};
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_DGRAM;
	if (getaddrinfo(hostname, std::to_string(ntohs(sin.sin_port)).c_str(), &hints, &result) != 0) { //0 here is to auto select FromPort
		return WSAGetLastError();
	}
	in_addr SendInterface{ ((sockaddr_in*)result->ai_addr)->sin_addr };
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	group_req grstruct{};
	grstruct.gr_group = *((sockaddr_storage*)&sin);
	GetBestInterfaceEx((sockaddr*)&sin, &grstruct.gr_interface);
	if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &no, sizeof(no)) != 0) {
		return WSAGetLastError();
	}
	if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, (char*)&SendInterface, sizeof(SendInterface)) != 0) {
		return WSAGetLastError();
	}
	if (setsockopt(s, IPPROTO_IP, MCAST_JOIN_GROUP, (char*)&grstruct, sizeof(grstruct)) != 0) {
		return WSAGetLastError();
	}
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) != 0) {
		return WSAGetLastError();
	}
	if (bind(s, result->ai_addr, (int)result->ai_addrlen) != 0) {
		return WSAGetLastError();
	}
	for (char& i : playername) {
		i = static_cast<char>(tolower(i));
	}
	playername[0] = static_cast<char>(toupper(playername[0]));
	memcpy_s(tsqr.name, 16, playername, 16);
	freeaddrinfo(result);
	return 0;
}
int prepareRoom(sockaddr_in& sin, SOCKET& s, Square& tsqr) {
	sin.sin_family = AF_INET;
	char hostname[256]{}; //Is actually DNS hostname, can't be playername because it has 255 characters limit
	char playername[16]{}; //Is actually computername because it has 15 characters limit
	DWORD playernamesize{ sizeof(playername) };
	if (GetComputerNameA(playername, &playernamesize) == 0) {
		return GetLastError();
	};
	if (gethostname(hostname, sizeof(hostname)) != 0) {
		return WSAGetLastError();
	}
	addrinfo* result{}, hints{};
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_DGRAM;
	if (getaddrinfo(hostname, std::to_string(rng(49152, 65537)).c_str(), &hints, &result) != 0) { //0 here is to auto select FromPort
		return WSAGetLastError();
	}
	in_addr SendInterface{ ((sockaddr_in*)result->ai_addr)->sin_addr };
	sin.sin_port = ((sockaddr_in*)result->ai_addr)->sin_port; //rng select ToPort, htons because sockaddr_in is in network order
	if (inet_pton(AF_INET, def_addr, &sin.sin_addr) != 1) {
		return WSAGetLastError();
	}
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	group_req grstruct{};
	grstruct.gr_group = *((sockaddr_storage*)&sin);
	GetBestInterfaceEx((sockaddr*)&sin, &grstruct.gr_interface);
	if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &no, sizeof(no)) != 0) {
		return WSAGetLastError();
	}
	if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, (char*)&SendInterface, sizeof(SendInterface)) != 0) {
		return WSAGetLastError();
	}
	if (setsockopt(s, IPPROTO_IP, MCAST_JOIN_GROUP, (char*)&grstruct, sizeof(grstruct)) != 0) {
		return WSAGetLastError();
	}
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) != 0) {
		return WSAGetLastError();
	}
	if (bind(s, result->ai_addr, (int)result->ai_addrlen) != 0) {
		return WSAGetLastError();
	}
	for (char& i : playername) {
		i = static_cast<char>(tolower(i));
	}
	playername[0] = static_cast<char>(toupper(playername[0]));
	memcpy_s(tsqr.name, 16, playername, 16);
	freeaddrinfo(result);
	return 0;
}
int pUpdate(sockaddr_in& sin, SOCKET& s, Square& tsqr) {
	static Position_Update last{};
	if (last != tsqr) {
		msg tosend{ std::move(Position_Update{tsqr}) };
		sendto(s, (char*)&tosend, sizeof(msg), 0, (sockaddr*)&sin, sizeof(sin));
		last = tsqr;
		return WSAGetLastError();
	}
}
int iReq(sockaddr_in& sin, SOCKET& s, Square& tsqr) {
	msg tosend{ std::move(Info_Request{tsqr}) };
	sendto(s, (char*)&tosend, sizeof(msg), 0, (sockaddr*)&sin, sizeof(sin));
	return WSAGetLastError();
}
int iRep(sockaddr_in& sin, SOCKET& s, Square& tsqr, uint_least8_t newindex) {
	msg tosend{ std::move(Info_Reply{tsqr, newindex}) };
	sendto(s, (char*)&tosend, sizeof(msg), 0, (sockaddr*)&sin, sizeof(sin));
	return WSAGetLastError();
}
int lReq(sockaddr_in& sin, SOCKET& s, uint_least8_t thisindex) {
	msg tosend{ std::move(thisindex) };
	sendto(s, (char*)&tosend, sizeof(msg), 0, (sockaddr*)&sin, sizeof(sin));
	return WSAGetLastError();
}
int processMsg(SOCKET& s, Sqrc& c, Square& tsqr, sockaddr_in& sin) {
	static std::set<uint_least8_t> indexset{};
	static uint_least8_t store{ []() {for (uint_least8_t i{ 1 }; i <= max_player; i++) {
		indexset.insert(indexset.end(), i);
	}; return (unsigned char)0; }()};
	char recvd[sizeof(msg)]{};
	recvfrom(s, recvd, sizeof(msg), 0, 0, 0);
	if (WSAGetLastError() == 0 && recvd != nullptr) {
		msg message{ *(msg*)recvd };
		switch (message.index()) {
		case 0: {
			Position_Update res{ std::get<0>(message) };
			if (c.contains(res.index)) {
				std::cout << "Received position update from index: " << static_cast<int>(res.index) << '\n';
				(*c.at(res.index)).posx = res.x;
				(*c.at(res.index)).posy = res.y;
			}
			break;
		}
		case 1: {
			Info_Request res{ std::get<1>(message) };
			if (std::find_if(c.begin(), c.end(), [&](std::pair<const uint_least8_t, std::unique_ptr<Square>>& sqrpair)->bool {return strcmp(res.name, sqrpair.second->name) == 0;}) == c.end()) {
				std::cout << "Received iReq: \n";
				std::cout << "RGB: " << static_cast<int>(res.r) << ',' << static_cast<int>(res.g) << ',' << static_cast<int>(res.b) << "\n";
				std::cout << "Name: " << res.name << '\n';
				uint_least8_t newindex{ *indexset.begin() };
				c.try_emplace(newindex, std::make_unique<Square>(res.name, newindex, res.r, res.g, res.b));
				iRep(sin, s, tsqr, newindex);
				indexset.erase(newindex);
			}
			break;
		}
		case 2: {
			Info_Reply res{ std::get<2>(message) };
			if (!c.contains(res.tsqr.index)) {
				std::cout << "Received iRep: \n";
				std::cout << "Newindex: " << static_cast<int>(res.nIndex) << '\n';
				std::cout << "Init pos: " << res.tsqr.posx << ',' << res.tsqr.posy << '\n';
				c.try_emplace(res.tsqr.index, std::make_unique<Square>(res.tsqr));
				if (res.nIndex != store) {
					tsqr.index = res.nIndex;
					indexset.erase(res.nIndex);
					store = res.nIndex;
				}
			}
			break;
		}
		case 3: {
			uint_least8_t res{ std::get<3>(message) };
			std::cout << "Received leaving notification for index:  " << static_cast<int>(res) << '\n';
			c.erase(res);
			indexset.emplace(res);
			std::cout << "Index " << static_cast<int>(res) << " is now available.\n";
			break;
		}
		}
		return 0;
	}
	return WSAGetLastError();
}
