#pragma once

#include "component.h"
#include <unordered_map>
#include "audioConfig.h"
#include "smoothMixer.h"
#include "deClicker.h"
#include <unordered_map>
#include <memory>
#include <vector>


struct AudioRig
{
	std::uint64_t idCounter = 1;

	constexpr static int SPEAKER_ID = 1;

	std::unordered_map< std::uint64_t, std::unique_ptr<Component>> components;

	std::vector<Link> links;

	SmoothMixer mixer{(int)sampleRate};
	DeClicker deClicker;

	float resultMix[bufferFrames * 2];

	void init();

	void simulate(int frames);

	std::uint64_t addOscilator(Vector2 position);

	std::uint64_t addPlus(Vector2 position);

	std::uint64_t addLfo(Vector2 position);

	std::uint64_t addVolume(Vector2 position);

	std::uint64_t addSplitter(Vector2 position);

	std::uint64_t addQuantizer(Vector2 position);

	std::uint64_t addLowPass(Vector2 position);

	std::uint64_t addReverb(Vector2 position);

	void addLink(Link link);

	void removeLinkFromOutputNode(std::uint64_t id, int position);

};