#include "basicOscilator.h"
#include <cmath>
#include "audioConfig.h"


float BasicOscilator::update(float frequency)
{
	const float twoPi = 6.283185307179586f;

	// advance phase
	phase += frequency / sampleRate;
	if (phase >= 1.0f) phase -= 1.0f;

	//return std::sinf(twoPi * phase);


	switch (type)
	{
		case sineWave:
			return std::sinf(twoPi * phase);

		case sawToothWave:
			// naive bipolar saw: -1..+1
			// (band-limit later if needed)
			return 2.0f * phase - 1.0f;

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
		default:
			return 0.0f;
	}

	return 0;

}
