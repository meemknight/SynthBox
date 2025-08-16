#pragma once
#include <component.h>
#include <audioConfig.h>
#include <knob.h>
#include <basicOscilator.h>

struct Lfo: public Component
{


	Vector2 getSize()
	{
		return Vector2{2,2};
	}

	BasicOscilator oscilator;

	Rectangle getScreenRect()
	{
		Rectangle rect = {position.x,position.y,getSize().x, getSize().y};
		rect.x += rect.width * 0.115;
		rect.y += rect.height * 0.115;
		rect.width *= 0.75;
		rect.height *= 0.42;
		return rect;
	}

	void render(AssetManager &assetManager)
	{
		Rectangle rect = {position.x,position.y,getSize().x, getSize().y};

		DrawTexturePro(assetManager.lfo,
			{0,0, (float)assetManager.lfo.width, (float)assetManager.lfo.height},
			rect, {0,0}, 0, WHITE);

		frecKnob.render(assetManager, position);

		minKnob.render(assetManager, position);
		maxKnob.render(assetManager, position);

		rect = getScreenRect();

		drawSineGraph(rect, getFrequency(), minKnob.value, maxKnob.value,
			0, {}, PURPLE, 0.02, oscilator.type);

	}

	float phase = 0.f;
	//float freq = 261.6256f; // C4

	Knob frecKnob{Vector2{0.68,1.43}, 0.44};

	Knob minKnob{Vector2{1.08,1.45}, 0.24, -1};
	Knob maxKnob{Vector2{1.34,1.45}, 0.24,  1};

	void uiUpdate(Vector2 mousePos)
	{

		frecKnob.update(mousePos, position);
		minKnob.update(mousePos, position);
		maxKnob.update(mousePos, position);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
			CheckCollisionPointRec(mousePos, getScreenRect()))
		{
			oscilator.cycleNextVersion();

		}
	}

	constexpr static float lowestNote = 0.01;
	constexpr static float highestNote = 25;

	float getFrequency()
	{
		float freq = frecKnob.linearRemapWithBias(inputs[0].input, lowestNote, highestNote);
		return freq;
	}

	void audioUpdate()
	{
		float freq = getFrequency();

		outputs[0].output = oscilator.update(freq);
	}

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.28,1.45};
		}

		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{1.68,1.45};
		}

		return {};
	}
		
		


};