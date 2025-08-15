#include "knob.h"
#include "assetManager.h"
#include <iostream>

void Knob::update(Vector2 mousePos, Vector2 parentPosition)
{

	Vector2 pos = {position.x + parentPosition.x, position.y + parentPosition.y};

	if (CheckCollisionPointCircle(mousePos, pos, size * 0.48))
	{
		highlight = true;
	}
	else
	{
		highlight = false;
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && highlight)
	{
		dragged = true;
		dragStart = mousePos;
	}

	if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		dragged = false;
	}

	if (dragged)
	{
		float delta = mousePos.x - dragStart.x;
		delta *= 0.2; //speed
		dragStart = mousePos;

		value += delta;
	}

	if (value < minValue) { value = minValue; }
	if (value > maxValue) { value = maxValue; }


}

static inline float clampf(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }

void Knob::render(AssetManager &assetManager, Vector2 parentPosition)
{
	Rectangle rect = {};

	rect.x = position.x + parentPosition.x;
	rect.y = position.y + parentPosition.y;
	rect.width = size;
	rect.height = size;

	float t = (value - minValue) / (maxValue - minValue);
	t = clampf(t, 0.0f, 1.0f);
	float angle = -160.0f * (1.0f - 2.0f * t);

	Vector2 origin = {rect.width * 0.5f, rect.height * 0.5f};

	Color color = LIGHTGRAY;
	if (highlight)
	{
		color = WHITE;
	}

	DrawTexturePro(assetManager.knob, 
		{0,0, (float)assetManager.knob.width, (float)assetManager.knob.height},
		rect, origin, angle, color);


}

float Knob::linearRemap(float toMin, float toMax)
{
	return ::linearRemap(value, minValue, maxValue, toMin, toMax);
}

float Knob::linearRemapWithBias(float bias, float toMin, float toMax)
{
	float val = value + bias;
	if (val < minValue) { val = minValue; }
	if (val > maxValue) { val = maxValue; }

	return ::linearRemap(val, minValue, maxValue, toMin, toMax);
}

float lerp(float a, float b, float r)
{
	return a * (1 - r) + b * r;
}

float linearRemap(float val, float fromMin, float fromMax, float toMin, float toMax)
{
	float rez = val;
	rez -= fromMin;

	rez /= (fromMax - fromMin);
	//rez is between 0 and 1 now

	rez *= (toMax - toMin);
	rez += toMin;

	return rez;
}
