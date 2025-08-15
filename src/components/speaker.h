#pragma once
#include <component.h>
#include <audioConfig.h>

#include <smoothMixer.h>
#include <deClicker.h>


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
		if (!input.pluggedIn)
		{
			output.output = 0;
		}
		else
		{
			float rez = input.input;
			
			//volume
			output.output = rez;
		}



		//mixer.process(&output.output, 1);
		//deClicker.process(&output.output, 1);


	}




};