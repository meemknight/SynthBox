#include "button.h"
#include <assetManager.h>


void Button::update(Vector2 mousePos, Vector2 parentPosition)
{
	
	Vector2 pos = {position.x + parentPosition.x, position.y + parentPosition.y};

	if (CheckCollisionPointCircle(mousePos, pos, size * 0.48))
	{
		highlight = true;
	}
	else
	{
		highlight = false;
	}

	if (highlight && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		on = !on;
	}

}

void Button::render(AssetManager &assetManager, Vector2 parentPosition)
{
	Rectangle rect = {};

	rect.x = position.x + parentPosition.x;
	rect.y = position.y + parentPosition.y;
	rect.width = size;
	rect.height = size;

	Vector2 origin = {rect.width * 0.5f, rect.height * 0.5f};

	Color color = highlight ? WHITE : LIGHTGRAY;
	Texture2D texture = on ? assetManager.buttonOn : assetManager.buttonOff;

	DrawTexturePro(texture,
		{0,0, (float)texture.width, (float)texture.height},
		rect, origin, 0, color);


}
