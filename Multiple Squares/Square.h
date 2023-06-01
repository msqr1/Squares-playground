#pragma once
#include <unordered_map>
#include <bitset>
#include <memory>
struct Square
{
	Square(const char* name, uint_least8_t index, uint_least8_t r, uint_least8_t g, uint_least8_t b);
	Square() = default;
	char name[16]{};
	double posx{};
	double posy{};
	uint_least8_t r{};
	uint_least8_t g{};
	uint_least8_t b{};
	uint_least8_t index{};
};
typedef std::unordered_map <uint_least8_t, std::unique_ptr<Square>> Sqrc;
