#pragma once
#include <component.h>
#include <audioConfig.h>
#include <knob.h>
#include <drawGraph.h>

struct Oscilator: public Component
{


	Vector2 getSize()
	{
		return Vector2{2,2};
	}


	void render(AssetManager &assetManager)
	{
		Rectangle rect = {position.x,position.y,getSize().x, getSize().y};

		DrawTexturePro(assetManager.oscilator,
			{0,0, (float)assetManager.oscilator.width, (float)assetManager.oscilator.height},
			rect, {0,0}, 0, WHITE);

		rect.x += rect.width * 0.06;
		rect.y += rect.height * 0.06;
		rect.width *= 0.82;
		rect.height *= 0.5;

		drawSineGraph(rect, getFrequency(), -1, 1);

		frecKnob.render(assetManager, position);

	}

	float phase = 0.f;
	//float freq = 261.6256f; // C4

	Knob frecKnob{Vector2{0.68,1.43}, 0.44};

	void uiUpdate(Vector2 mousePos)
	{

		frecKnob.update(mousePos, position);

	}

	constexpr static float lowestNote = 20;
	constexpr static float highestNote = 2000;

	float getFrequency()
	{
		float freq = frecKnob.linearRemapWithBias(inputs[0].input, lowestNote, highestNote);
		return freq;
	}

	void audioUpdate()
	{
		const float twoPi = 6.283185307179586f;

		float freq = getFrequency();
		

		// advance phase
		phase += freq / sampleRate;
		if (phase >= 1.0f) phase -= 1.0f;

		outputs[0].output = sinf(twoPi * phase);
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