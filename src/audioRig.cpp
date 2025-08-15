#include "audioRig.h"
#include <components/speaker.h>
#include <components/oscilator.h>

void AudioRig::init()
{
	*this = {};

	Speaker speaker;
	speaker.position.x = 1;

	components[idCounter++] = std::make_unique<Speaker>(speaker);
}

std::uint64_t AudioRig::addOscilator(Vector2 position)
{

	Oscilator oscilator;
	oscilator.position = position;

	components[idCounter] = std::make_unique<Oscilator>(oscilator);

	return idCounter++;
}


void AudioRig::simulate(int frames)
{

	//pre step check plugged in stuff
	for (auto &c : components)
	{
		c.second->input.pluggedIn = false;

		for (auto &l : links)
		{

			if (l.toComponent == c.first)
			{
				//todo also jack id
				c.second->input.pluggedIn = true;
				break;
			}
		}
	}


	for (unsigned int i = 0; i < frames; ++i)
	{

		//step 1 run the update function
		for (auto &c : components)
		{
			c.second->audioUpdate();

			//reset the input
			c.second->input.input = 0;
		}

		//step 2 move the audio to the next component
		for (auto l : links)
		{

			auto fromComponent = components.find(l.fromComponent);
			auto toComponent = components.find(l.toComponent);

			if(fromComponent == components.end() || 
				toComponent == components.end()
				)
			{
				//todo remove link
				continue;
			}

			toComponent->second->input.input = fromComponent->second->output.output;

		}

		//step 3 get the result in the buffer
		resultMix[i] = components[SPEAKER_ID]->output.output;
	}

	mixer.process(resultMix, frames);
	deClicker.process(resultMix, frames);






}