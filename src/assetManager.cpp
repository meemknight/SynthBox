#include "assetManager.h"	


void AssetManager::loadAll()
{

	oscilator = LoadTexture(RESOURCES_PATH "oscilator.png");
	speaker = LoadTexture(RESOURCES_PATH "speaker.png");
	circle = LoadTexture(RESOURCES_PATH "circle.png");
	plus = LoadTexture(RESOURCES_PATH "plus.png");
	knob = LoadTexture(RESOURCES_PATH "knob.png");
	lfo = LoadTexture(RESOURCES_PATH "lfo.png");
	volume = LoadTexture(RESOURCES_PATH "volume.png");
	splitter = LoadTexture(RESOURCES_PATH "splitter.png");
	quantizer = LoadTexture(RESOURCES_PATH "quantizer.png");

	buttonOn = LoadTexture(RESOURCES_PATH "buttonOn.png");
	buttonOff = LoadTexture(RESOURCES_PATH "buttonOff.png");
	lowPass = LoadTexture(RESOURCES_PATH "lowPass.png");
	reverb = LoadTexture(RESOURCES_PATH "reverb.png");
	sequencer = LoadTexture(RESOURCES_PATH "sequencer.png");
	slider = LoadTexture(RESOURCES_PATH "slider.png");
	sliderOn = LoadTexture(RESOURCES_PATH "sliderOn.png");
	
	
}