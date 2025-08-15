#pragma once
#include <component.h>
#include <audioConfig.h>


struct Oscilator: public Component
{


	Vector2 getSize()
	{
		return Vector2{2,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.oscilator,
			{0,0, (float)assetManager.oscilator.width, (float)assetManager.oscilator.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

	}

	float phase = 0.f;
	float freq = 261.6256f; // C4


	void audioUpdate()
	{
		const float twoPi = 6.283185307179586f;

		// advance phase
		phase += freq / sampleRate;
		if (phase >= 1.0f) phase -= 1.0f;

		outputs[0].output = sinf(twoPi * phase) * 0.5f;
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