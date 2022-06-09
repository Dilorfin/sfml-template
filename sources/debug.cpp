#include "debug.hpp"

float pixelsToMeters(const float px)
{
	return px / SCALE;
}

float metersToPixels(const float meters)
{
	return meters * SCALE;
}