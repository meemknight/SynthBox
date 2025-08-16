#pragma once
#include <raylib.h>
#include <knob.h>

struct AssetManager;

struct Slider
{
	Slider() {};
	Slider(Vector2 position):position(position) {};
	Slider(Vector2 position, float size):position(position), size(size) {};
	Slider(Vector2 position, float size, float runLength):position(position), size(size), runLength(runLength) {};
	Slider(Vector2 position, float size, float runLength, float value):position(position), size(size), runLength(runLength), value(value) {};


	Vector2 position = {};
	float size = 0.2;
	float runLength = 1;
	float value = 0;
	bool highlight = false;
	bool dragging = false;

	void update(Vector2 mousePos, Vector2 parentPosition);

	void render(AssetManager &assetManager, Vector2 parentPosition);

	float linearRemap(float toMin, float toMax) 
	{
		return ::linearRemap(value, -1, 1, toMin, toMax);
	};

};




