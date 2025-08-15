#include "assetManager.h"	


void AssetManager::loadAll()
{

	oscilator = LoadTexture(RESOURCES_PATH "oscilator.png");
	speaker = LoadTexture(RESOURCES_PATH "speaker.png");
	circle = LoadTexture(RESOURCES_PATH "circle.png");
	plus = LoadTexture(RESOURCES_PATH "plus.png");
	knob = LoadTexture(RESOURCES_PATH "knob.png");
	lfo = LoadTexture(RESOURCES_PATH "lfo.png");

}