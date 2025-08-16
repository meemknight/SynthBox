#pragma once
#include <component.h>
#include <audioConfig.h>
#include <knob.h>
#include <drawGraph.h>
#include <basicOscilator.h>

struct Oscilator: public Component
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

		DrawTexturePro(assetManager.oscilator,
			{0,0, (float)assetManager.oscilator.width, (float)assetManager.oscilator.height},
			rect, {0,0}, 0, WHITE);

		rect = getScreenRect();

		drawSineGraph(rect, getFrequency(), -1, 1, 0, {}, GREEN, 0.02, oscilator.type);

		frecKnob.render(assetManager, position);

	}


	//float freq = 261.6256f; // C4

	Knob frecKnob{Vector2{0.68,1.43}, 0.44};

	void uiUpdate(Vector2 mousePos)
	{

		frecKnob.update(mousePos, position);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
			CheckCollisionPointRec(mousePos, getScreenRect()))
		{
			oscilator.cycleNextVersion();

		}

	}

	constexpr static float lowestNote = 20;
	constexpr static float highestNote = 2000;

	float getFrequency()
	{
		float freq = frecKnob.linearRemapWithBias(inputs[0].input, lowestNote, highestNote);
		return freq;
	}

	static constexpr float freqToCV(float f)
	{
		float t = (f - lowestNote) / (highestNote - lowestNote);      // 0..1
		float cv = t * 2.0f - 1.0f;             // -> -1..1
		return std::clamp(cv, -1.0f, 1.0f);
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