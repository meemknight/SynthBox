#include "slider.h"
#include <assetManager.h>


void Slider::update(Vector2 mousePos, Vector2 parentPosition)
{
	// Top of the slider track (position represents TOP, centered on X)
	Vector2 top = {position.x + parentPosition.x, position.y + parentPosition.y};

	// Map value [-1..1] -> offset [0..runLength], with +1 at top, -1 at bottom
	auto valueToOffset = [&](float v)
	{
		return (1.0f - v) * (runLength * 0.5f); // v=1 -> 0; v=0 -> run/2; v=-1 -> run
	};

	float knobOffset = valueToOffset(value);

	// Knob center
	Vector2 knobCenter = {top.x, top.y + knobOffset};

	// Hover / highlight over the knob (use same circle hit as Button)
	float knobRadius = size * 0.48f;
	highlight = CheckCollisionPointCircle(mousePos, knobCenter, knobRadius);

	// Begin dragging if mouse pressed while hovering
	if (highlight && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		dragging = true;

	// Update while dragging
	if (dragging)
	{
		// t in [0..1] along the track from top to bottom
		float t = (mousePos.y - top.y) / runLength;
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;

		// Map t [0..1] -> value [-1..1] with top=+1, bottom=-1
		value = 1.0f - 2.0f * t;
	}

	// Stop dragging on release
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		dragging = false;

}

void Slider::render(AssetManager &assetManager, Vector2 parentPosition)
{
	// Recompute positions like in update
	Vector2 top = {position.x + parentPosition.x, position.y + parentPosition.y};

	auto valueToOffset = [&](float v)
	{
		return (1.0f - v) * (runLength * 0.5f);
	};

	float knobOffset = valueToOffset(value);
	Vector2 knobCenter = {top.x, top.y + knobOffset};

	// Draw only the knob
	Rectangle dest = {};
	dest.x = knobCenter.x;
	dest.y = knobCenter.y;
	dest.width = size;
	dest.height = size;

	Vector2 origin = {dest.width * 0.5f, dest.height * 0.5f};

	Color color = (highlight || dragging) ? WHITE : LIGHTGRAY;
	Texture2D texture = assetManager.slider;

	DrawTexturePro(
		texture,
		{0, 0, (float)texture.width, (float)texture.height},
		dest,
		origin,
		0.0f,
		color
	);

}
