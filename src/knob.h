#pragma once
#include <raylib.h>

struct AssetManager;

struct Knob
{
	Knob() {};
	Knob(Vector2 position):position(position) {};
	Knob(Vector2 position, float size):position(position), size(size) {};
	Knob(Vector2 position, float size, float value):
		position(position), size(size), value(value) {};

	Vector2 position = {};

	float value = 0;
	float minValue = -1;
	float maxValue =  1;
	float size = 0.25;

	bool dragged = 0;
	bool highlight = false;
	Vector2 dragStart = {};

	void update(Vector2 mousePos, Vector2 parentPosition);

	void render(AssetManager &assetManager, Vector2 parentPosition);

	float linearRemap(float toMin, float toMax);
	float linearRemapWithBias(float bias, float toMin, float toMax);

};

float lerp(float a, float b, float r);

float linearRemap(float val, float fromMin, float fromMax, float toMin, float toMax);