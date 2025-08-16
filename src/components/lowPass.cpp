#include "lowPass.h"

void LowPass::audioUpdate()
{
	// --- toggle resonance here ---
	const bool resonanceOn = resonance.on; // set false to disable resonance

	// Inputs
	float inSignal = inputs[0].input;

	// Cutoff CV: [-1..1]  -> map exponentially to ~[20..20000] Hz
	float cutoffCV = cutoffKnob.value + inputs[1].input;
	cutoffCV = clamp(cutoffCV, -1.0f, 1.0f);
	float t = (cutoffCV + 1.0f) * 0.5f;               // 0..1
	const float fmin = 20.0f, fmax = 20000.0f;
	float cutoffHz = fmin * powf(fmax / fmin, t);     // exponential mapping

	// Resonance (Q): 0.707 ~ no emphasis (Butterworth), higher => more resonance
	float Q = resonanceOn ? 6.2f : 0.707f;            // adjust to taste

	if (fabsf(cutoffHz - prevCut) > 1e-3f || fabsf(Q - prevQ) > 1e-4f)
	{
		prevCut = cutoffHz; prevQ = Q;

		// Clamp usable range
		float sr = sampleRate;                         // assumes you have this in scope
		cutoffHz = std::max(10.0f, std::min(cutoffHz, 0.45f * sr));

		float w0 = 2.0f * 3.14159265f * cutoffHz / sr;
		float c = cosf(w0), s = sinf(w0);
		float alpha = s / (2.0f * std::max(0.1f, Q));

		float b0n = (1.0f - c) * 0.5f;
		float b1n = 1.0f - c;
		float b2n = (1.0f - c) * 0.5f;
		float a0 = 1.0f + alpha;
		float a1n = -2.0f * c;
		float a2n = 1.0f - alpha;

		b0 = b0n / a0; b1 = b1n / a0; b2 = b2n / a0;
		a1 = a1n / a0; a2 = a2n / a0;
	}

	// Process one sample
	float x = inSignal;
	float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

	// Shift history
	x2 = x1; x1 = x;
	y2 = y1; y1 = y;

	// Output (clamped just in case)
	outputs[0].output = std::max(-1.0f, std::min(1.0f, y));
}