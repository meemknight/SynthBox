#include "audioRig.h"
#include <components/speaker.h>
#include <components/oscilator.h>
#include <components/plus.h>
#include <components/lfo.h>
#include <components/volume.h>

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

std::uint64_t AudioRig::addPlus(Vector2 position)
{
	Plus component;
	component.position = position;
	components[idCounter] = std::make_unique<Plus>(component);
	return idCounter++;
}

std::uint64_t AudioRig::addLfo(Vector2 position)
{
	Lfo component;
	component.position = position;
	components[idCounter] = std::make_unique<Lfo>(component);
	return idCounter++;
}

std::uint64_t AudioRig::addVolume(Vector2 position)
{
	Volume component;
	component.position = position;
	components[idCounter] = std::make_unique<Volume>(component);
	return idCounter++;
}

void AudioRig::addLink(Link link)
{

	auto fromComponent = components.find(link.fromComponent);
	auto toComponent = components.find(link.toComponent);

	if (fromComponent == components.end() ||
		toComponent == components.end()
		)
	{
		return;
	}

	links.erase(
		std::remove_if(links.begin(), links.end(),
		[&](const auto &l)
		{
			return (l.fromComponent == link.fromComponent &&
				l.fromOutputNumber == link.fromOutputNumber)
				|| (l.toComponent == link.toComponent &&
				l.toInputNumber == link.toInputNumber);
		}),
		links.end());


	links.push_back(link);

}

void AudioRig::removeLinkFromOutputNode(std::uint64_t id, int position)
{

	links.erase(
		std::remove_if(links.begin(), links.end(),
		[&](const auto &l)
		{
			return (l.fromComponent == id &&
				l.fromOutputNumber == position);
		}),
		links.end());


}


void AudioRig::simulate(int frames)
{

	//pre step check plugged in stuff
	for (auto &c : components)
	{

		for (int index = 0; index < MAX_INPUTS; index++)
		{

			if (c.second->getInputPosition(index) == std::nullopt) { break; }

			c.second->inputs[index].pluggedIn = false;

			for (auto &l : links)
			{

				if (l.toComponent == c.first && l.toInputNumber == index)
				{
					c.second->inputs[index].pluggedIn = true;
					break;
				}
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
			for (int index = 0; index < MAX_INPUTS; index++)
			{
				if (c.second->getInputPosition(index) == std::nullopt) { break; }
				c.second->inputs[index].input = 0;
			}
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

			toComponent->second->inputs[l.toInputNumber].input = fromComponent->second->outputs[l.fromOutputNumber].output;

		}

		//step 3 get the result in the buffer
		resultMix[i] = components[SPEAKER_ID]->outputs[0].output;
	}

	mixer.process(resultMix, frames);
	deClicker.process(resultMix, frames);






}