#pragma once
#include <component.h>
#include <audioConfig.h>
#include <optional>
#include <knob.h>

//INPUT 0 -> OUTPUT 0 1 2 3

struct Sequencer: public Component
{


	Vector2 getSize()
	{
		return Vector2{4,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.sequencer,
			{0,0, (float)assetManager.sequencer.width, (float)assetManager.sequencer.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

		speedKnob.render(assetManager, position);
	}

	Knob speedKnob{{0.23, 1.52}, 0.22};

	void uiUpdate(Vector2 mousePos)
	{
		speedKnob.update(mousePos, position);
	}

	void audioUpdate()
	{


		if (inputs[0].pluggedIn)
		{
			outputs[0].output = inputs[0].input;
			outputs[1].output = inputs[0].input;
			outputs[2].output = inputs[0].input;
			outputs[3].output = inputs[0].input;
		}
		else
		{
			outputs[0].output = 0;
			outputs[1].output = 0;
			outputs[2].output = 0;
			outputs[3].output = 0;
		}

	}

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.22, 1.78};
		}


		return {};
	}

	std::optional<Vector2> getOutputPosition(int index)
	{
		
		//beat
		if (index == 0) { return Vector2{3.1,  1.55}; } //trigger
		if (index == 1) { return Vector2{3.41,  1.55}; } //hold
		if (index == 2) { return Vector2{3.72,  1.55}; } //note

		//offbeat
		if (index == 3) { return Vector2{3.1,  1.85}; } //trigger
		if (index == 4) { return Vector2{3.41,  1.85}; } //hold
		if (index == 5) { return Vector2{3.72,  1.85}; } //note

		return {};
	}


};