#pragma once
#include <raylib.h>

struct AssetManager;

struct Button
{
	Button() {};
	Button(Vector2 position):position(position) {};
	Button(Vector2 position, float size):position(position), size(size) {};
	Button(Vector2 position, float size, bool on):position(position), size(size), on(on) {};


	Vector2 position = {};
	float size = 0.4;
	bool on = false;
	bool highlight = false;

	void update(Vector2 mousePos, Vector2 parentPosition);

	void render(AssetManager &assetManager, Vector2 parentPosition);

};

