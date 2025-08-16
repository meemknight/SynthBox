#pragma once


enum WaveTypes
{
	sineWave = 0,
	sawToothWave,
	triangleWave,
	squareWave,
	WAVES_TYPES_COUNT
};

struct BasicOscilator
{

	float phase = 0;
	int type = sineWave;


	float update(float frequency);

	void cycleNextVersion()
	{
		type++;
		type %= WAVES_TYPES_COUNT;
	}



};