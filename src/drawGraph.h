#pragma once

#include "raylib.h"
#include <math.h>
#include <algorithm>

// Helper clamp
static inline float clamp01(float x) { return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); }

// If you have your own remap, pass it in; signature must match.
// float Remap(float x, float inMin, float inMax, float outMin, float outMax)
void drawSineGraph(Rectangle rect,
	float frequency,        // arbitrary units, e.g. 0.5..2000 (display-compressed)
	float minVal, float maxVal,
	float phase = 0.0f,     // radians
	Color bg = {18, 20, 24, 0},
	Color wave = GREEN,
	float thicknessPx = 0.02);


