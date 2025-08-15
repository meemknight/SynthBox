#include "raylib.h"
#include <cmath>
#include <algorithm>

static inline float clampf(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }

void drawSineGraph(Rectangle rect, float frequency, float minVal, float maxVal,
	float phase, Color bg, Color wave, float thicknessPx)
{
	if (rect.width <= 0.0f || rect.height <= 0.0f) return;

	// Background (world space)
	DrawRectangleRec(rect, bg);
	//BeginScissorMode(rect.x, rect.y, rect.width, rect.height);

	// Ensure valid range
	if (minVal == maxVal) maxVal = minVal + 0.000001f;
	if (minVal > maxVal) std::swap(minVal, maxVal);
	const float range = maxVal - minVal;

	// Visual frequency compression (keeps ultra-high freqs visible but sane)
	float cycles = std::pow(frequency, 0.25);
	const float minCycles = 0.1f;
	const float softCap = 20.0f;
	if (cycles < minCycles) cycles = minCycles;
	if (cycles > softCap)   cycles = softCap + logf(1.0f + (cycles - softCap));

	// Adaptive segment count:
	// - decoupled from screen pixels, works for tiny rects in world units
	// - scale with rect width and with cycles to avoid under-sampling
	const float baseDensityPerUnit = 64.0f;          // segments per world-unit width
	int segments = (int)std::ceilf(std::max(rect.width, 0.25f) * baseDensityPerUnit
		+ cycles * 24.0f);
	if (segments < 32) { segments = 32; }
	if (segments > 16384) { segments = 16384; }
	
	const float twoPi = 6.283185307179586f;

	auto evalY = [&](float t01)
	{
		float s = sinf(twoPi * (t01 * cycles) + phase);          // [-1,1]
		float v = minVal + (s + 1.0f) * 0.5f * range;            // remap to [minVal,maxVal]
		//v = clampf(v, minVal, maxVal);                         // clamp for drawing
		float y = rect.y + rect.height - ((v + 1) / 2) * rect.height;
		return y;

	};

	float xPrev = rect.x;
	float yPrev = evalY(0.0f);

	const float thick = (thicknessPx > 0.0f) ? thicknessPx : 1.0f;

	for (int i = 1; i <= segments; ++i)
	{
		float t = (float)i / (float)segments;                    // [0,1] across rect
		float x = rect.x + t * rect.width;
		float y = evalY(t);

		// Keep strictly inside rect horizontally (guards against tiny FP drift)
		if (x > rect.x + rect.width) x = rect.x + rect.width;

		DrawLineEx(Vector2{xPrev, yPrev}, Vector2{x, y}, thick, wave);

		xPrev = x;
		yPrev = y;
	}

	//EndScissorMode();
}
