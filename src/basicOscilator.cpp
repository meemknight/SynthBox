#include "basicOscilator.h"
#include <cmath>
#include "audioConfig.h"
#include <algorithm>


static inline float centsToRatio(float cents)
{
	return std::pow(2.0f, cents / 1200.0f);
}

float BasicOscilator::update(float frequency)
{
	const float twoPi = 6.283185307179586f;

	// advance phase
	phase += frequency / sampleRate;

	if (phase >= 1.0f) 
	{
		phase -= 1.0f; 
		
		if (type == steppedRandom)
		{
			std::uniform_real_distribution<float> dist(-1, 1);
			randomValueHold = dist(rng);
		}

	};

	//return std::sinf(twoPi * phase);


	switch (type)
	{
		case sineWave:
			return std::sinf(twoPi * phase);

		case sawToothWave:
			// naive bipolar saw: -1..+1
			// (band-limit later if needed)
			return 2.0f * phase - 1.0f;

		case reversedSawToothWave:
			return 2.0f * (1-phase) - 1.0f;

			//todo move into another module
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
			return sum * (1.0f / (float)n) * 3;
		}

		case triangleWave:
		{
			// bipolar triangle via folded saw: -1..+1
			float t = 2.0f * phase - 1.0f;           // -1..+1 saw
			float tri = 2.0f * (1.0f - std::fabs(t)) - 1.0f; // -1..+1 triangle
			return tri;
		}

		case squareWave:
		{
			// pulse with duty = pulseWidth (0..1), output -1/+1
			const float pw = 0.5f;
			return (phase < pw) ? 1.0f : -1.0f;
		}

		case whiteNoise:
		{
			std::uniform_real_distribution<float> dist(-1, 1);
			return dist(rng);
		}

		case steppedRandom:
		{
			return randomValueHold;
		}

		default:
			return 0.0f;
	}

	return 0;

}
