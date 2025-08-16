#include "raylib.h"
#include <cmath>
#include <algorithm>
#include "drawGraph.h"

static inline float clampf(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }

void drawSineGraph(Rectangle rect, float frequency, float minVal, float maxVal,
    float phase, Color bg, Color wave, float thicknessPx, int waveType)
{
    if (rect.width <= 0.0f || rect.height <= 0.0f) return;

    DrawRectangleRec(rect, bg);

    if (minVal == maxVal) maxVal = minVal + 0.000001f;
    if (minVal > maxVal) std::swap(minVal, maxVal);
    const float range = maxVal - minVal;

    float cycles = std::pow(frequency, 0.25f);
    const float minCycles = 0.1f;
    const float softCap = 20.0f;
    if (cycles < minCycles) cycles = minCycles;
    if (cycles > softCap)   cycles = softCap + logf(1.0f + (cycles - softCap));

    const float baseDensityPerUnit = 64.0f;
    int segments = (int)std::ceilf(std::max(rect.width, 0.25f) * baseDensityPerUnit
        + cycles * 24.0f);
    if (segments < 32) { segments = 32; }
    if (segments > 16384) { segments = 16384; }

    const float twoPi = 6.283185307179586f;
    const float invTwoPi = 1.0f / twoPi;
    const float pulseWidth = 0.5f; // square duty (0.5 = square)

    auto evalY = [&](float t01)
    {
        // phaseArg in radians, and fractional phase p in [0,1)
        float phaseArg = twoPi * (t01 * cycles) + phase;
        float p = phaseArg * invTwoPi;      // cycles + phase (in cycles)
        p = p - floorf(p);                  // frac

        float s = 0.0f;                     // waveform in [-1,1]
        switch (waveType)
        {
            default: // fallthrough to sine
            case sineWave: // sine
                s = sinf(phaseArg);
                break;

            case sawToothWave: // saw (naive, bipolar)
                s = 2.0f * p - 1.0f;
                break;

            case triangleWave: // triangle (from folded saw), bipolar
            {
                float saw = 2.0f * p - 1.0f;             // -1..+1
                s = 2.0f * (1.0f - fabsf(saw)) - 1.0f;   // -1..+1
            } break;

            case squareWave: // square / pulse (bipolar)
                s = (p < pulseWidth) ? 1.0f : -1.0f;
                break;
        }

        // map from [-1,1] -> [minVal,maxVal]
        float v = minVal + (s + 1.0f) * 0.5f * range;

        // draw-space Y (kept exactly like your original math)
        float y = rect.y + rect.height - ((v + 1) / 2) * rect.height;
        return y;
    };

    float xPrev = rect.x;
    float yPrev = evalY(0.0f);
    const float thick = (thicknessPx > 0.0f) ? thicknessPx : 1.0f;

    for (int i = 1; i <= segments; ++i)
    {
        float t = (float)i / (float)segments;
        float x = rect.x + t * rect.width;
        float y = evalY(t);
        if (x > rect.x + rect.width) x = rect.x + rect.width;
        DrawLineEx(Vector2{ xPrev, yPrev }, Vector2{ x, y }, thick, wave);
        xPrev = x;
        yPrev = y;
    }
}