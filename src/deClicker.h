// Zero-latency post de-clicker (slew limiter)
struct DeClicker
{
	// Max allowed step per sample (full-scale is 1.0). Smaller = smoother, larger = snappier.
	float maxDelta = 0.02f;   // ~1–2 ms to settle a full-scale step at 48 kHz
	float y = 0.0f;           // last output (state)

	DeClicker() {}
	explicit DeClicker(float maxDeltaPerSample): maxDelta(maxDeltaPerSample) {}

	inline float processSample(float x)
	{
		float d = x - y;
		if (d > maxDelta) d = maxDelta;
		if (d < -maxDelta) d = -maxDelta;
		y += d;
		return y;
	}

	void process(float *buf, int frames)
	{
		for (int i = 0; i < frames; ++i)
		{
			float x = buf[i];
			float d = x - y;
			if (d > maxDelta) d = maxDelta;
			if (d < -maxDelta) d = -maxDelta;
			y += d;
			buf[i] = y;
		}
	}

	// Optional: call when you know the stream will start from silence to avoid a ramp from old state
	void reset(float to = 0.0f) { y = to; }
};
