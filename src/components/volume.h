#pragma once
#include <component.h>
#include <audioConfig.h>

#include <knob.h>

//INPUT 0 -> OUTPUT 0
//INPUT 1 -> OUTPUT 1
//Input 2 for volume

struct Volume: public Component
{


	Vector2 getSize()
	{
		return Vector2{1,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.volume,
			{0,0, (float)assetManager.volume.width, (float)assetManager.volume.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

		knob.render(assetManager, position);

	}

	Knob knob{{0.68, 0.99}, 0.44, 0.5, 0, 1};

	void uiUpdate(Vector2 mousePos)
	{

		knob.update(mousePos, position);

	}

	void audioUpdate()
	{

		float inputVolume = inputs[2].input;

		if (!inputs[0].pluggedIn)
		{
			outputs[0].output = 0;
		}
		else
		{
			float rez = inputs[0].input;
			rez *= knob.linearRemapWithBias(inputVolume, 0, 1);
			outputs[0].output = rez;
		}


		if (!inputs[1].pluggedIn)
		{
			outputs[1].output = 0;
		}
		else
		{
			float rez = inputs[1].input;
			rez *= knob.linearRemapWithBias(inputVolume, 0, 1);
			outputs[1].output = rez;
		}

	}

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.27,0.245};
		}
		if (index == 1)
		{
			return Vector2{0.73,0.245};
		}
		if (index == 2)
		{
			return Vector2{0.25,1.1};
		}


		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.27,1.6};
		}

		if (index == 1)
		{
			return Vector2{0.73,1.6};
		}
		return {};
	}


};