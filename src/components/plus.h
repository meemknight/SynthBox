#pragma once
#include <component.h>
#include <audioConfig.h>
#include <optional>

//INPUT 0 1 2 3 -> OUTPUT 0

struct Plus: public Component
{


	Vector2 getSize()
	{
		return Vector2{1,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.plus,
			{0,0, (float)assetManager.plus.width, (float)assetManager.plus.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

	}


	void audioUpdate()
	{
		outputs[0].output = 0;

		for (int i = 0; i < 4; i++)
		{
			if (inputs[i].pluggedIn)
			{
				outputs[0].output += inputs[i].input;
			}
		}

	}

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
			{ return Vector2{0.25, 0.25}; }
		
		if (index == 1)
			{ return Vector2{0.75, 0.25}; }

		if (index == 2)
			{ return Vector2{0.27, 0.67}; }
		
		if (index == 3)
			{ return Vector2{0.73, 0.67}; }

		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.5,1.6};
		}

		return {};
	}


};