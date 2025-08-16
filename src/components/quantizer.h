#pragma once
#include <component.h>
#include <audioConfig.h>
#include <optional>
#include <knob.h>
#include <button.h>
#include <components/oscilator.h>

//INPUT 0 CV     1 PULS    -> OUTPUT 0

struct Quantizer: public Component
{


	Vector2 getSize()
	{
		return Vector2{2,2};
	}

	Knob freqKnob{Vector2{1.14,0.33}, 0.34};
	Knob speedKnob{Vector2{1.14,0.73}, 0.34};

	constexpr static int NOTES_COUNT = 15;
	Button noteButtons[NOTES_COUNT] = 
	{
		Button({0.25,1.75}, 0.16),
		Button({0.33,1.46}, 0.16), //black
		Button({0.44,1.75}, 0.16),
		Button({0.55,1.46}, 0.16), //black
		Button({0.63,1.75}, 0.16),
		Button({0.82,1.75}, 0.16),

		Button({0.88,1.46}, 0.16), //black

		Button({1.01,1.75}, 0.16),
		Button({1.11,1.46}, 0.16), //black
		Button({1.20,1.75}, 0.16),
		Button({1.31,1.46}, 0.16), //black
		Button({1.39,1.75}, 0.16),
		Button({1.56,1.75}, 0.16),
		Button({1.65,1.46}, 0.16), //black
		Button({1.75,1.75}, 0.16),
	};

	struct NoteCV { const char *name; float freq; float cv; };

	static constexpr NoteCV kNotesC4_to_D5[NOTES_COUNT] = {
		{ "C4",  261.63f, Oscilator::freqToCV(261.63f) },  // ~= -0.75593
		{ "C#4", 277.18f, Oscilator::freqToCV(277.18f) },  // ~= -0.74022
		{ "D4",  293.66f, Oscilator::freqToCV(293.66f) },  // ~= -0.72358
		{ "D#4", 311.13f, Oscilator::freqToCV(311.13f) },  // ~= -0.70593
		{ "E4",  329.63f, Oscilator::freqToCV(329.63f) },  // ~= -0.68724
		{ "F4",  349.23f, Oscilator::freqToCV(349.23f) },  // ~= -0.66744
		{ "F#4", 369.99f, Oscilator::freqToCV(369.99f) },  // ~= -0.64647
		{ "G4",  392.00f, Oscilator::freqToCV(392.00f) },  // ~= -0.62424
		{ "G#4", 415.30f, Oscilator::freqToCV(415.30f) },  // ~= -0.60071
		{ "A4",  440.00f, Oscilator::freqToCV(440.00f) },  // ~= -0.57576
		{ "A#4", 466.16f, Oscilator::freqToCV(466.16f) },  // ~= -0.54933
		{ "B4",  493.88f, Oscilator::freqToCV(493.88f) },  // ~= -0.52133
		{ "C5",  523.25f, Oscilator::freqToCV(523.25f) },  // ~= -0.49167
		{ "C#5", 554.37f, Oscilator::freqToCV(554.37f) },  // ~= -0.46023
		{ "D5",  587.33f, Oscilator::freqToCV(587.33f) },  // ~= -0.42694
	};

	void render(AssetManager &assetManager)
	{

		DrawTexturePro(assetManager.quantizer,
			{0,0, (float)assetManager.quantizer.width, (float)assetManager.quantizer.height},
			{position.x,position.y,getSize().x, getSize().y}, {0,0}, 0, WHITE);

		freqKnob.render(assetManager, position);
		speedKnob.render(assetManager, position);

		for (int i = 0; i < NOTES_COUNT; i++)
		{
			noteButtons[i].render(assetManager, position);
		}

	}

	void uiUpdate(Vector2 mousePos)
	{

		freqKnob.update(mousePos, position);
		speedKnob.update(mousePos, position);
		
		for (int i = 0; i < NOTES_COUNT; i++)
		{
			noteButtons[i].update(mousePos, position);
		}

	}

	void audioUpdate()
	{
		outputs[0].output = 0;

		for (int i = 0; i < NOTES_COUNT; i++)
		{
			if (noteButtons[i].on)
			{
				outputs[0].output = kNotesC4_to_D5[i].cv;
			}
		}

	}

	std::optional<Vector2> getInputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{0.35, 0.31};
		}

		if (index == 1)
		{
			return Vector2{0.35, 0.73};
		}

		return {};
	}


	std::optional<Vector2> getOutputPosition(int index)
	{
		if (index == 0)
		{
			return Vector2{1.70,1.0};
		}

		return {};
	}


};