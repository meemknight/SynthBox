#pragma once
#include <component.h>
#include <audioConfig.h>

#include <smoothMixer.h>
#include <deClicker.h>

//INPUT 0 -> OUTPUT 0

struct Speaker: public Component
{


	Vector2 getSize()
	{
		return Vector2{1,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.speaker,
			{0,0, (float)assetManager.speaker.width, (float)assetManager.speaker.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

	}

	
	//SmoothMixer mixer{(int)sampleRate};
	//DeClicker deClicker;


	void audioUpdate()
	{
		if (!inputs[0].pluggedIn)
		{
			outputs[0].output = 0;
		}
		else
		{
			float rez = inputs[0].input;
			
			//volume
			outputs[0].output = rez;
		}



		//mixer.process(&output.output, 1);
		//deClicker.process(&output.output, 1);

	}

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.5,1.48};
		}

		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		return {};
	}


};