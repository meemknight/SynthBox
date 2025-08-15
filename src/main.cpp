// Single-file: raylib + ImGui + ImGuiKnobs, audio via CALLBACK, ASCII only

#include "raylib.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <vector>

#pragma region imgui
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"
#include <imgui-knobs.h>
#pragma endregion

#include "windowsStuff.h"
#include "component.h"
#include "components/oscilator.h"
#include "components/speaker.h"
#include <audioConfig.h>

std::atomic<bool> gHeldQ{false};

SmoothMixer mixer(sampleRate);
DeClicker deClicker;
Camera2D camera;
Oscilator oscilator;
Speaker speaker;
AssetManager assetManager;


void MyAudioCB(void *bufferData, unsigned int frames)
{
	// stream format: 16-bit mono
	int16_t *out = (int16_t *)bufferData;


	const float freq = 261.6256f; // C4
	static float phase = 0.0f;

	const float freqC = 261.6256f;
	const float freqE = 329.6276f;
	const float freqG = 391.9954f;

	static float phaseC = 0.0f;
	static float phaseE = 0.0f;
	static float phaseG = 0.0f;

	const float twoPi = 6.283185307179586f;

	bool held = gHeldQ.load(std::memory_order_relaxed);

	static float mix[bufferFrames];

	for (unsigned int i = 0; i < frames; ++i)
	{

		//if (held)
		//{
		//	// advance phase
		//	phase += freq / sampleRate;
		//	if (phase >= 1.0f) phase -= 1.0f;
		//
		//	phaseC += freqC / sampleRate; if (phaseC >= 1.0f) phaseC -= 1.0f;
		//	phaseE += freqE / sampleRate; if (phaseE >= 1.0f) phaseE -= 1.0f;
		//	phaseG += freqG / sampleRate; if (phaseG >= 1.0f) phaseG -= 1.0f;
		//
		//	mix[i] = sinf(twoPi * phase) * 0.5f;
		//
		//	mix[i] = (sinf(twoPi * phaseC) +
		//		sinf(twoPi * phaseE) +
		//		sinf(twoPi * phaseG)) * 0.5f;
		//
		//}
		//else
		//{
		//	mix[i] = 0;
		//}

		oscilator.audioUpdate();

		speaker.input.pluggedIn = true;
		speaker.input.thisFrame = oscilator.output.output;

		speaker.audioUpdate();

		mix[i] = speaker.output.output;
		
		
	}

	mixer.process(mix, frames);
	deClicker.process(mix, frames);

	for (unsigned int i = 0; i < frames; ++i)
	{
		float clamped = std::max(-1.0f, std::min(1.0f, mix[i]));
		int vi = (int)lrintf(clamped * 32767.0f);
		out[i] = (int16_t)std::max(-32768, std::min(32767, vi));
	}

}

int main()
{
	enableTimeBeginPeriod();

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(900, 520, "raylib synth");

#pragma region imgui
	rlImGuiSetup(true);
	imguiThemes::green();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 2;
#pragma endregion

	// Audio
	mixer.setLookaheadMs(1);
	const int sampleSize = 16;
	const int channels = 1;
	SetAudioStreamBufferSizeDefault(bufferFrames);

	InitAudioDevice();
	AudioStream stream = LoadAudioStream(sampleRate, sampleSize, channels);

	// switch to callback (pull model)
	SetAudioStreamCallback(stream, MyAudioCB);
	PlayAudioStream(stream);
	SetAudioStreamVolume(stream, 0.7f);
	SetTargetFPS(120);


#pragma region init game
	
	assetManager.loadAll();

	camera.target = {0,0};
	camera.rotation = 0;
	camera.zoom = 100;

	oscilator.position.x = -2;
	speaker.position.x = 0;

#pragma endregion



	while (!WindowShouldClose())
	{

		camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};


		// update control atomics (no audio work here)
		gHeldQ.store(IsKeyDown(KEY_Q), std::memory_order_relaxed);

		// UI
		BeginDrawing();
		ClearBackground(GRAY);



		BeginMode2D(camera);
		{

			oscilator.render(assetManager);
			speaker.render(assetManager);

		}
		EndMode2D();


		rlImGuiBegin();
		//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		//ImGui::Begin("Synth");
		//ImGui::Text("Hold Q to play (callback)");
		//ImGui::Text("SR: %f  BufferFrames: %d", sampleRate, bufferFrames);
		//ImGui::End();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();

	UnloadAudioStream(stream);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
