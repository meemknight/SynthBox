#pragma once
#include <component.h>
#include <audioConfig.h>
#include <optional>

//INPUT 0 -> OUTPUT 0 1 2 3

struct Splitter: public Component
{


	Vector2 getSize()
	{
		return Vector2{1,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.splitter,
			{0,0, (float)assetManager.splitter.width, (float)assetManager.splitter.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

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
			return Vector2{0.5,0.3};
		}


		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{

		if (index == 0)
		{
			return Vector2{0.29, 1.16};
		}

		if (index == 1)
		{
			return Vector2{0.71, 1.16};
		}

		if (index == 2)
		{
			return Vector2{0.29, 1.58};
		}

		if (index == 3)
		{
			return Vector2{0.71, 1.58};
		}

		return {};
	}


};