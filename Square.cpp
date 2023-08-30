#include "Square.h"
Square::Square(const char* name, uint_least8_t index, uint_least8_t r, uint_least8_t g, uint_least8_t b) : r(r), g(g), b(b), index(index) {
	memcpy_s(this->name, 16, name, 16);
}
