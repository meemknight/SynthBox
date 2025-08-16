#pragma once
#include <component.h>
#include <audioConfig.h>
#include <optional>
#include <knob.h>
#include <slider.h>
#include <random.h>

//INPUT 0 -> OUTPUT 0 1 2 3

struct Sequencer: public Component
{

	Sequencer()
	{
		for (int i = 0; i < 8; i++)
		{
			slidersBeat[i] = {Vector2{0.26f + (float)i * 0.2f, 0.25}, 0.18, 0.4};
		}

		for (int i = 0; i < 8; i++)
		{
			slidersBeat[i + 8] = {Vector2{0.26f + (float)i * 0.2f, 0.85}, 0.18, 0.4};
		}

		for (int i = 0; i < 8; i++)
		{
			notesBeat[i] = {Vector2{2.32f + (float)i * 0.2f, 0.25}, 0.18, 0.4};
		}

		for (int i = 0; i < 8; i++)
		{
			notesBeat[i + 8] = {Vector2{2.32f + (float)i * 0.2f, 0.85}, 0.18, 0.4};
		}
	}


	Vector2 getSize()
	{
		return Vector2{4,2};
	}


	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.sequencer,
			{0,0, (float)assetManager.sequencer.width, (float)assetManager.sequencer.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

		speedKnob.render(assetManager, position);

		for (auto &s : slidersBeat)
		{
			s.render(assetManager, position);
		}

		for (auto &s : notesBeat)
		{
			s.render(assetManager, position);
		}
	}

	constexpr static int SLIDERS_COUNT = 16;

	Knob speedKnob{{0.23, 1.52}, 0.22};
	Slider slidersBeat[SLIDERS_COUNT];
	Slider notesBeat[SLIDERS_COUNT];
	std::minstd_rand rng{std::random_device{}()};

	void uiUpdate(Vector2 mousePos)
	{
		speedKnob.update(mousePos, position);

		for (auto &s : slidersBeat)
		{
			s.update(mousePos, position);
		}

		for (auto &s : notesBeat)
		{
			s.update(mousePos, position);
		}

	}

	float phase = 0.0f;           // [0..16)
	int   lastStep = -1;          // last whole-step index
	bool  beatActive = false;     // current step is "beat"
	bool  offActive = false;     // current step is "offbeat" (only if beat didn't hit)
	float stepNote = 0.0f;      // note of current step
	int   beatTrigCnt = 0;        // trigger pulse counters
	int   offTrigCnt = 0;

	void audioUpdate()
	{
		// --- Params ---
		float speed = speedKnob.linearRemapWithBias(inputs[0].input, 0.05f, 4.0f);
		const int steps = 16;
		const float incPerSample = speed * (float)steps / sampleRate;

		// how many ticks a trigger stays at +1
		const int TRIG_LEN = 2;

		// --- Advance phase ---
		phase += incPerSample;
		while (phase >= steps) phase -= steps;
		while (phase < 0.0f)   phase += steps;

		int step = (int)phase; // 0..15

		// --- Entering a new step? ---
		if (step != lastStep)
		{
			lastStep = step;

			// Probability the BEAT hits on this step
			float p = slidersBeat[step].linearRemap(0.0f, 1.0f);
			beatActive = getRandomChance(rng, p);
			offActive = !beatActive;                 // offbeat is the complement

			stepNote = notesBeat[step].value;

			// fire triggers
			beatTrigCnt = beatActive ? TRIG_LEN : 0;
			offTrigCnt = offActive ? TRIG_LEN : 0;
		}

		// --- Defaults ---
		outputs[0].output = -1.0f; // beat trigger
		outputs[1].output = -1.0f; // beat hold
		outputs[2].output = 0.0f; // beat note (0 when inactive)

		outputs[3].output = -1.0f; // offbeat trigger
		outputs[4].output = -1.0f; // offbeat hold
		outputs[5].output = 0.0f; // offbeat note

		// --- Triggers (short pulses to +1) ---
		if (beatTrigCnt > 0) { outputs[0].output = 1.0f; --beatTrigCnt; }
		if (offTrigCnt > 0) { outputs[3].output = 1.0f; --offTrigCnt; }

		// --- Holds (+1 while active on current step) ---
		if (beatActive)
		{
			outputs[1].output = 1.0f;
			outputs[2].output = stepNote;
		}
		if (offActive)
		{
			outputs[4].output = 1.0f;
			outputs[5].output = stepNote;
		}

		//optional always write the note
		outputs[2].output = stepNote;
		outputs[5].output = stepNote;

	}
	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.22, 1.78};
		}


		return {};
	}

	std::optional<Vector2> getOutputPosition(int index)
	{
		
		//beat
		if (index == 0) { return Vector2{3.1,  1.55}; } //trigger
		if (index == 1) { return Vector2{3.41,  1.55}; } //hold
		if (index == 2) { return Vector2{3.72,  1.55}; } //note

		//offbeat
		if (index == 3) { return Vector2{3.1,  1.85}; } //trigger
		if (index == 4) { return Vector2{3.41,  1.85}; } //hold
		if (index == 5) { return Vector2{3.72,  1.85}; } //note

		return {};
	}


};