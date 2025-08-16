#pragma once
#include <random>



inline int getRandomNumber(std::minstd_rand &rng, int min, int max)
{
	return rng() % (max + 1 - min) + min;
}

inline float getRandomNumberFloat(std::minstd_rand &rng, float min, float max)
{
	return (getRandomNumber(rng, 0, 100000) / 100000.f) * (max - min) + min;
}

inline bool getRandomChance(std::minstd_rand &rng, float chance)
{
	float dice = getRandomNumberFloat(rng, 0.0, 1.0);
	return dice <= chance;
}