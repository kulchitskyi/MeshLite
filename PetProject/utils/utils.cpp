#include "utils.h"

glm::vec3 Utils::idToColor(uint32_t id)
{
	uint8_t r = (id >> 16) & 0xFF;
	uint8_t g = (id >> 8) & 0xFF;
	uint8_t b = id & 0xFF;
	return glm::vec3(r, g, b) / 255.0f;
}

uint32_t Utils::colorToID(const uint8_t r, const uint8_t g, const uint8_t b) 
{
	return (r << 16) | (g << 8) | b;
}

glm::vec3 Utils::RGBtoVec3(unsigned char r, unsigned char g, unsigned char b) 
{
	return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
}


