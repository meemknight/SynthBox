#include "reverb.h"

void Reverb::audioUpdate()
{
	float x = inputs[0].input;

	// Params in [-1,1]
	float pSize = clamp(inputs[1].input + sizeKnob.value, -1.0f, 1.0f);
	float pPreDelay = clamp(inputs[2].input + preDelayKnob.value, -1.0f, 1.0f);
	float pDamping = clamp(inputs[3].input + dampingKnob.value, -1.0f, 1.0f);
	float pTone = clamp(inputs[4].input + toneKnob.value, -1.0f, 1.0f);
	float pWet = clamp(inputs[5].input + wetKnob.value, -1.0f, 1.0f);

	// One-time init of buffers (max sizes so we never reallocate in audio)
	if (!rvbInit)
	{
		// Pre-delay up to 150 ms
		int maxPreSamp = (int)std::ceil(0.150f * sampleRate);
		preBuf.assign(std::max(1, maxPreSamp), 0.0f);
		preIdx = 0;

		// Comb lines (parallel). Give headroom beyond the largest size scaling.
		// Base delays (ms) chosen to be mutually prime-ish for decorrelation.
		const float combMs[NUM_COMBS] = {29.7f, 37.1f, 41.1f, 43.7f};
		for (int i = 0; i < NUM_COMBS; ++i)
		{
			// Max with size scale up to ~1.3x, plus some margin
			float maxMs = combMs[i] * 1.35f + 10.0f;
			int maxLen = std::max(1, (int)std::ceil(maxMs * 0.001f * sampleRate));
			combBuf[i].assign(maxLen, 0.0f);
			combIdx[i] = 0;
			combLP[i] = 0.0f;
		}

		// Allpass lines (series)
		const float apMs[NUM_APS] = {5.0f, 1.7f};
		for (int i = 0; i < NUM_APS; ++i)
		{
			int len = std::max(1, (int)std::ceil(apMs[i] * 0.001f * sampleRate));
			apBuf[i].assign(len, 0.0f);
			apIdx[i] = 0;
		}

		toneZ = 0.0f;
		sizeSm = 0.0f; // smooth start
		rvbInit = true;
	}

	auto unipolar = [](float v) { return 0.5f * (v + 1.0f); }; // [-1,1] -> [0,1]
	float size01 = unipolar(pSize);
	float wet01 = unipolar(pWet);
	float damping01 = unipolar(pDamping);
	float tone01 = unipolar(pTone);
	float preMs = 0.0f + 100.0f * unipolar(pPreDelay); // 0..100 ms user range

	// ---- Smooth room size (avoid zipper/pitching when changing) ----
	// smaller slew => slower change
	const float sizeSlew = 0.0015f;
	sizeSm += (size01 - sizeSm) * sizeSlew;

	// ---- Pre-delay (circular buffer) ----
	preBuf[preIdx] = x;
	preIdx = (preIdx + 1) % (int)preBuf.size();
	int preSamp = std::min((int)preBuf.size() - 1, std::max(0, (int)std::round(preMs * 0.001f * sampleRate)));
	int preRead = preIdx - preSamp; if (preRead < 0) preRead += (int)preBuf.size();
	float preOut = preBuf[preRead];

	// ---- Parallel combs with fractional delay + damping in feedback ----
	// Base comb delays (ms)
	const float combBaseMs[NUM_COMBS] = {29.7f, 37.1f, 41.1f, 43.7f};

	// Target T60 from size (stable decay mapping, avoids boom/instability with longer delays)
	// Map Size 0..1 to about 0.4..8.0 s
	float T60 = 0.4f * std::pow(8.0f / 0.4f, sizeSm);

	// Damping in feedback loop (larger => darker tail)
	float dampCoef = std::clamp(0.15f + 0.80f * damping01, 0.0f, 0.98f);

	float combSum = 0.0f;
	for (int i = 0; i < NUM_COMBS; ++i)
	{
		int   idx = combIdx[i];
		int   bufLen = (int)combBuf[i].size();

		// Delay length in samples, scaled by smoothed size
		float ms = combBaseMs[i] * (0.70f + 0.60f * sizeSm);   // ~0.7x..1.3x
		float dSamp = ms * 0.001f * sampleRate;
		dSamp = std::clamp(dSamp, 1.0f, (float)(bufLen - 2));  // keep safe for interp

		// Fractional read (linear interpolation)
		float readPosF = (float)idx - dSamp;
		while (readPosF < 0.0f) readPosF += (float)bufLen;
		int   r0 = (int)readPosF;
		int   r1 = r0 + 1; if (r1 >= bufLen) r1 = 0;
		float frac = readPosF - (float)r0;
		float d0 = combBuf[i][r0];
		float d1 = combBuf[i][r1];
		float d = d0 + (d1 - d0) * frac; // delayed sample

		// Per-comb feedback to hit T60 for THIS delay length
		float fb = std::pow(10.0f, -3.0f * (dSamp / sampleRate) / std::max(0.05f, T60));
		fb = std::min(fb, 0.985f);

		// Damped feedback (LP inside loop)
		float ylp = (1.0f - dampCoef) * d + dampCoef * combLP[i];
		combLP[i] = ylp;

		// Write input + feedback
		combBuf[i][idx] = preOut + fb * ylp;

		// Advance
		combIdx[i] = (idx + 1) % bufLen;

		// Tap comb output (delayed sample) into sum
		combSum += d;
	}
	combSum *= (1.0f / (float)NUM_COMBS);

	// ---- Series allpasses for diffusion ----
	float apIn = combSum;
	const float apG = 0.5f; // allpass coefficient
	for (int i = 0; i < NUM_APS; ++i)
	{
		int   len = (int)apBuf[i].size();
		int   idx = apIdx[i];
		float v = apBuf[i][idx];

		float y = v - apG * apIn;
		apBuf[i][idx] = apIn + apG * y;

		apIdx[i] = (idx + 1) % len;
		apIn = y;
	}
	float wetSig = apIn;

	// ---- Post-tone low-pass on wet (simple one-pole), tone 500..8000 Hz ----
	float toneHz = 500.0f * std::pow(8000.0f / 500.0f, tone01);
	float toneAlpha = std::exp(-2.0f * 3.14159265f * toneHz / sampleRate);
	toneZ = toneAlpha * toneZ + (1.0f - toneAlpha) * wetSig;
	wetSig = toneZ;

	// ---- Mix ----
	float y = wetSig * wet01 + x * (1.0f - wet01);
	outputs[0].output = std::max(-1.0f, std::min(1.0f, y));
}