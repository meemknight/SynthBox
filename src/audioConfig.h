#pragma once


constexpr float sampleRate = 32000;  //44100
constexpr int bufferFrames = 160;

inline float clamp(float a, float min, float max)
{
	if (a < min) { a = min; }
	if (a > max) { a = max; }
	return a;
}
