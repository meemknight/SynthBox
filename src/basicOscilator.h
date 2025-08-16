#pragma once
#include <random>


enum WaveTypes
{
	sineWave = 0,
	sawToothWave,
	reversedSawToothWave,
	triangleWave,
	superSaw,
	squareWave,
	whiteNoise,
	steppedRandom,
	WAVES_TYPES_COUNT
};

struct BasicOscilator
{

	float phase = 0;
	int type = sineWave;
	std::minstd_rand0 rng{std::random_device{}()};
	float randomValueHold = 0;

	float update(float frequency);

	void cycleNextVersion()
	{
		type++;
		type %= WAVES_TYPES_COUNT;
	}

	// ---- Super-saw params/state ----
	static constexpr int kMaxSuper = 9;      // up to 9 detuned saws
	int   superCount = 7;                  // use odd number: 3,5,7,9
	float detuneCents = 25.0f;              // total spread; ±12.5c each side if 7 voices
	float ssPhase[kMaxSuper] = {};           // per-voice phases [0,1)
	bool  ssInit = false;


};