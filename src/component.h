#pragma once
#include <raylib.h>
#include "assetManager.h"
#include <cstdint>

struct Input
{

	float input = 0;
	bool pluggedIn = 0;

};

struct Output
{
	float output = 0;
};

struct Link
{
	std::uint64_t fromComponent = 0;
	std::uint64_t toComponent = 0;

	int fromOutputNumber = 0;
	int toOutputNumber = 0;
};


struct Component
{

	Input input;
	Output output;


	Vector2 position = {0,0};

	virtual Vector2 getSize() = 0;

	virtual void render(AssetManager &assetManager) = 0;

	virtual void audioUpdate() = 0;

};

