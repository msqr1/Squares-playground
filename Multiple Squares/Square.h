#pragma once
#include <string>
#include <unordered_map>
#include <memory>
struct Square
{
	Square(const char* name, uint_least8_t index, uint_least8_t r, uint_least8_t g, uint_least8_t b);
	Square() = default;
	uint_least8_t r{};
	uint_least8_t g{};
	uint_least8_t b{};
	double posx{};
	double posy{};
	uint_least8_t index{};
	char name[16]{};
};
typedef std::unordered_map <uint_least8_t, std::unique_ptr<Square>> Sqrc;
