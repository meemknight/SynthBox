#pragma once
#include <raylib.h>
#include "assetManager.h"
#include <cstdint>
#include <optional>	

struct Input
{

	float input = 0;
	bool pluggedIn = 0;

};

struct Output
{
	float output = 0;
};

struct Link
{
	std::uint64_t fromComponent = 0;
	std::uint64_t toComponent = 0;

	int fromOutputNumber = 0;
	int toInputNumber = 0;
};

constexpr static int MAX_INPUTS = 8;
constexpr static int MAX_OUTPUTS = 8;


struct Component
{

	Input inputs[MAX_INPUTS];
	Output outputs[MAX_OUTPUTS];

	Vector2 position = {0,0};

	virtual Vector2 getSize() = 0;

	virtual void render(AssetManager &assetManager) = 0;

	virtual void audioUpdate() = 0;

	virtual std::optional<Vector2> getInputPosition(int index) = 0;
	virtual std::optional<Vector2> getOutputPosition(int index) = 0;

};

