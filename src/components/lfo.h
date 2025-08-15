#pragma once
#include <component.h>
#include <audioConfig.h>
#include <knob.h>

struct Lfo: public Component
{


	Vector2 getSize()
	{
		return Vector2{2,2};
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

		rect.x += rect.width * 0.06;
		rect.y += rect.height * 0.06;
		rect.width *= 0.82;
		rect.height *= 0.5;

		drawSineGraph(rect, getFrequency(), minKnob.value, maxKnob.value);

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
		const float twoPi = 6.283185307179586f;

		float freq = getFrequency();

		// advance phase
		phase += freq / sampleRate;
		if (phase >= 1.0f) phase -= 1.0f;

		outputs[0].output = linearRemap(sinf(twoPi * phase), 
			-1, 1, minKnob.value, maxKnob.value);
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