#pragma once
#include <component.h>
#include <audioConfig.h>
#include <knob.h>
#include <basicOscilator.h>

struct Reverb: public Component
{


	Vector2 getSize()
	{
		return Vector2{2,2};
	}

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

		DrawTexturePro(assetManager.reverb,
			{0,0, (float)assetManager.reverb.width, (float)assetManager.reverb.height},
			rect, {0,0}, 0, WHITE);

		sizeKnob.render(assetManager, position);
		preDelayKnob.render(assetManager, position);
		dampingKnob.render(assetManager, position);
		toneKnob.render(assetManager, position);
		wetKnob.render(assetManager, position);


	}

	Knob sizeKnob{Vector2{0.24,0.73}, 0.35};
	Knob preDelayKnob{Vector2{0.62,0.73}, 0.35};
	Knob dampingKnob{Vector2{0.98,0.73}, 0.35};
	Knob toneKnob{Vector2{1.36,0.73}, 0.35};
	Knob wetKnob{Vector2{1.74,0.73}, 0.35};

	void uiUpdate(Vector2 mousePos)
	{

		sizeKnob.update(mousePos, position);
		preDelayKnob.update(mousePos, position);
		dampingKnob.update(mousePos, position);
		toneKnob.update(mousePos, position);
		wetKnob.update(mousePos, position);

	}

	void audioUpdate();

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.42,1.6};
		}

		if (index == 1) { return Vector2{0.24,1.05}; }
		if (index == 2) { return Vector2{0.62,1.05}; }
		if (index == 3) { return Vector2{0.98,1.05}; }
		if (index == 4) { return Vector2{1.36,1.05}; }
		if (index == 5) { return Vector2{1.74,1.05}; }

		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{1.59,1.60};
		}

		return {};
	}


	// --- persistent state (put these in your Reverb class/struct) ---
	bool   rvbInit = false;

	// Pre-delay
	std::vector<float> preBuf;
	int preIdx = 0;

	// combs (parallel)
	static constexpr int NUM_COMBS = 4;
	std::vector<float> combBuf[NUM_COMBS];
	int   combIdx[NUM_COMBS] = {0,0,0,0};
	float combLP[NUM_COMBS] = {0,0,0,0};  // damping state per comb

	// allpasses (series)
	static constexpr int NUM_APS = 2;
	std::vector<float> apBuf[NUM_APS];
	int   apIdx[NUM_APS] = {0,0};

	// post-tone filter state (per instance)
	float toneZ = 0.0f;

	// smoothed size parameter (per instance)
	float sizeSm = 0.0f;


};
