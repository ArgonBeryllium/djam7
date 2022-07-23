#pragma once
#include <cumt_things.h>

struct State;
struct Boxer : cumt::Thing2D
{
	State* state;
	Boxer* opponent;
	bool is_player;

	Boxer(bool is_player_ = false);

	bool setState(State* next, bool interrupt = true, bool auto_delete = true);
	void render() override;
	void update() override;
};
