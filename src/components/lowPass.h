#pragma once
#include <component.h>
#include <audioConfig.h>
#include <knob.h>
#include <basicOscilator.h>
#include <button.h>

struct LowPass: public Component
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

		DrawTexturePro(assetManager.lowPass,
			{0,0, (float)assetManager.lowPass.width, (float)assetManager.lowPass.height},
			rect, {0,0}, 0, WHITE);

		cutoffKnob.render(assetManager, position);
		resonance.render(assetManager, position);


	}

	Knob cutoffKnob{Vector2{1.08,1.27}, 0.38};
	Button resonance{Vector2{1.38,1.27}, 0.2};

	void uiUpdate(Vector2 mousePos)
	{

		cutoffKnob.update(mousePos, position);
		resonance.update(mousePos, position);

	}

	void audioUpdate();

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.24,1.34};
		}

		if (index == 1)
		{
			return Vector2{0.72,1.34};
		}

		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{1.66,1.30};
		}

		return {};
	}



	// --- RBJ biquad low-pass (Direct Form I) ---
	// Keep state and coefficients across calls
	float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
	float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;

	// Recompute coeffs when cutoff/Q change noticeably (cheap; you can always compute every call)
	float prevCut = -1.0f, prevQ = -1.0f;


};