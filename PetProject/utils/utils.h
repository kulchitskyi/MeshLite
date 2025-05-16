#pragma once
#include "glm/glm.hpp"

namespace Utils
{
	glm::vec3 idToColor(uint32_t id);

	uint32_t colorToID(const uint8_t r, const uint8_t b, const uint8_t g);

	glm::vec3 RGBtoVec3(unsigned char r, unsigned char g, unsigned char b);
}