#pragma once
#include <raylib.h>
#include "assetManager.h"


struct Input
{

	float thisFrame = 0;
	float lastFrame = 0;
	bool pluggedIn = 0;

};

struct Output
{
	float output = 0;
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