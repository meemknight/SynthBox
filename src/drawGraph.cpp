#include "raylib.h"
#include <cmath>
#include <algorithm>
#include "drawGraph.h"
#include "audioConfig.h"

static inline float centsToRatio(float cents)
{
	return std::pow(2.0f, cents / 1200.0f);
}

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
	std::minstd_rand0 rng(0);

	// ---- Super-saw params/state ----
	static constexpr int kMaxSuper = 9;      // up to 9 detuned saws
	int   superCount = 7;                  // use odd number: 3,5,7,9
	float detuneCents = 25.0f;              // total spread; ±12.5c each side if 7 voices
	float ssPhase[kMaxSuper] = {};           // per-voice phases [0,1)
	bool  ssInit = false;

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


			case reversedSawToothWave:
				s = 2.0f * (1-p) - 1.0f;
				break;

			case triangleWave: // triangle (from folded saw), bipolar
			{
				float saw = 2.0f * p - 1.0f;             // -1..+1
				s = 2.0f * (1.0f - fabsf(saw)) - 1.0f;   // -1..+1
			} break;

			case squareWave: // square / pulse (bipolar)
				s = (p < pulseWidth) ? 1.0f : -1.0f;
				break;

			case whiteNoise:
			{

				std::uniform_real_distribution<float> dist(-1, 1);
				s = dist(rng);
				break;
			}

			case superSaw:
			{
				// Lazy-init per-voice phases using your RNG
				if (!ssInit)
				{
					std::uniform_real_distribution<float> dist01(0.0f, 1.0f);
					int n = clamp(superCount, 1, kMaxSuper);
					for (int i = 0; i < n; ++i) ssPhase[i] = dist01(rng); // [0,1)
					ssInit = true;
				}

				int n = clamp(superCount, 1, kMaxSuper);
				if (n == 1)
				{
					// fall back to single saw
					return 2.0f * phase - 1.0f;
				}

				int   mid = n / 2;                // center index for odd n
				float halfSpread = detuneCents * 0.5f;   // +/- half spread in cents
				float sum = 0.0f;

				for (int i = 0; i < n; ++i)
				{
					int   k = i - mid;                               // -mid..0..+mid
					float norm = (n > 1) ? ((float)k / (float)mid) : 0.0f; // -1..+1
					float cents = norm * halfSpread;
					float ratio = centsToRatio(cents);

					// advance per-voice phase
					float incV = (frequency * ratio) / sampleRate;
					ssPhase[i] += incV;
					if (ssPhase[i] >= 1.0f) ssPhase[i] -= 1.0f;

					// naive saw
					float saw = 2.0f * ssPhase[i] - 1.0f;

					// slight center emphasis (optional; remove if you want flat)
					float w = 1.0f - 0.1f * std::fabs(norm);
					sum += saw * w;
				}

				// rough normalization
				s = (sum * (1.0f / (float)n)) * 3;

				break;
			}

			case steppedRandom:
			{
				// How many steps per cycle to show (tweak as you like)
				const int stepsPerCycle = 4;

				// Total phase in cycles (includes "phase" offset)
				float total = (t01 * cycles) + phase * invTwoPi;

				// Separate integer cycle index and fractional position within the cycle
				float cycIdxF = floorf(total);
				float frac = total - cycIdxF;

				// Step index within this cycle
				int stepIdx = (int)floorf(frac * stepsPerCycle);
				if (stepIdx >= stepsPerCycle) stepIdx = stepsPerCycle - 1;

				// Make a deterministic hash key per (cycle, step)
				uint32_t key = (uint32_t)((int)cycIdxF * stepsPerCycle + stepIdx);

				// Fast integer hash -> [0,1)
				auto hash01 = [](uint32_t k)
				{
					k ^= k >> 16;  k *= 0x7feb352dU;
					k ^= k >> 15;  k *= 0x846ca68bU;
					k ^= k >> 16;
					// take 24 high-ish bits for a float in [0,1)
					return (float)((k >> 8) & 0x00FFFFFFu) * (1.0f / 16777215.0f);
				};

				float r01 = hash01(key);       // 0..1
				s = r01 * 2.0f - 1.0f;         // -> -1..1 (bipolar)
			} break;
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