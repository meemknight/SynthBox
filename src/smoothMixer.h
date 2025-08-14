#include <vector>
#include <cmath>


// ================= SmoothMixer.h (single-file class) =================
struct SmoothMixer
{
	// tunables
	float targetRMS = 0.22f;   // desired RMS after AGC (0..1)
	float agcAttackMs = 5.0f;    // fast when getting louder (more notes)
	float agcReleaseMs = 300.0f;  // slow when getting quieter (avoid jumps)
	float maxGain = 8.0f;    // AGC gain limits
	float minGain = 0.125f;

	float lookaheadMs = 1.0f;    // limiter lookahead time
	float ceiling = 0.98f;   // limiter peak ceiling
	float limAttackMs = 0.5f;    // limiter gain-down speed
	float limReleaseMs = 50.0f;   // limiter gain-up speed
	float softClipK = 0.2f;    // 0..0.6+; 0 disables soft clip

	// runtime
	int   sampleRate = 44100;
	std::vector<float> delay;
	int   dlPos = 0;
	int   lookaheadSamp = 0;

	// envelopes/gains
	float rmsEnv = 0.0f;    // EMA of x^2
	float agcGain = 1.0f;
	float peakEnv = 0.0f;
	float limGain = 1.0f;

	SmoothMixer() {}
	explicit SmoothMixer(int sr) { init(sr); }

	void init(int sr)
	{
		sampleRate = sr;
		setLookaheadMs(lookaheadMs);
		rmsEnv = 0.0f; agcGain = 1.0f; peakEnv = 0.0f; limGain = 1.0f;
	}

	void setLookaheadMs(float ms)
	{
		lookaheadMs = ms;
		lookaheadSamp = std::max(1, (int)std::round(ms * 0.001f * sampleRate));
		int need = std::max(lookaheadSamp + 4, 2048);
		delay.assign(need, 0.0f);
		dlPos = 0;
	}

	static inline float msToCoef(float ms, int sr)
	{
		float tau = std::max(ms * 0.001f, 0.0001f);
		return expf(-1.0f / (tau * sr));   // per-sample smoothing coef
	}

	void process(float *mix, int frames)
	{
		// precompute coefs
		const float rmsAtkC = msToCoef(agcAttackMs, sampleRate);
		const float rmsRelC = msToCoef(agcReleaseMs, sampleRate);
		const float limAtkC = msToCoef(limAttackMs, sampleRate);
		const float limRelC = msToCoef(limReleaseMs, sampleRate);
		const float hold = 0.995f;

		for (int i = 0; i < frames; ++i)
		{
			float x = mix[i];

			// write to delay line for lookahead
			delay[dlPos] = x;

			// --- AGC (RMS-based auto gain) ---
			// update short-term RMS env
			float x2 = x * x;
			// choose coef depending on level change direction after applying current gain
			float currentRMS = sqrtf(std::max(rmsEnv, 1e-12f));
			float desiredGain = targetRMS / std::max(currentRMS, 1e-6f);
			desiredGain = std::max(desiredGain, minGain);
			desiredGain = std::min(desiredGain, maxGain);
			// update rmsEnv with EMA
			// faster when signal increases (attack), slower when decreases (release)
			float nextRmsEnv = (x2 > rmsEnv)
				? (rmsEnv + (1.0f - rmsAtkC) * (x2 - rmsEnv))
				: (rmsEnv + (1.0f - rmsRelC) * (x2 - rmsEnv));
			rmsEnv = nextRmsEnv;

			// smooth agcGain toward desiredGain
			if (desiredGain < agcGain) agcGain += (1.0f - rmsAtkC) * (desiredGain - agcGain);
			else                       agcGain += (1.0f - rmsRelC) * (desiredGain - agcGain);

			// --- Limiter detection (pre on delayed sample, post AGC) ---
			float preLim = x * agcGain;
			float a = fabsf(preLim);
			peakEnv = std::max(a, peakEnv * hold);
			float t = (peakEnv > ceiling) ? (ceiling / peakEnv) : 1.0f;
			// smooth limiter gain (fast down, slow up)
			if (t < limGain) limGain += (1.0f - limAtkC) * (t - limGain);
			else             limGain += (1.0f - limRelC) * (t - limGain);

			// read delayed sample and apply gains
			int readPos = dlPos - lookaheadSamp;
			if (readPos < 0) readPos += (int)delay.size();
			float y = delay[readPos] * agcGain * limGain;

			// optional soft clip as safety
			if (softClipK > 0.0f)
			{
				float k = softClipK;
				y = y * (1.0f - k * y * y);
			}

			mix[i] = y;

			// advance delay pointer; slowly decay peak env
			if (++dlPos >= (int)delay.size()) dlPos = 0;
			peakEnv *= 0.999f;
		}
	}
};
