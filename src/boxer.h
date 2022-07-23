#pragma once
#include <SDL2/SDL_render.h>
#include <cumt_things.h>
#include "assets.h"

struct State;
struct Boxer;
struct StateData
{
	Boxer* parent;
	float dur_idle_base = 1, dur_idle_range = .5;
	float dur_windup = .2;
	float dur_hit = .5;
	float dur_punch = .2;

	float strength = .1;

	SDL_Texture** tex_idle   = t_test_idle;
	SDL_Texture** tex_windup = t_test_windup;
	SDL_Texture** tex_hit    = t_test_hit;
	SDL_Texture** tex_punch  = t_test_punch;
};
struct Boxer : cumt::Thing2D
{
	State* state;
	StateData sd;
	Boxer* opponent;
	bool is_player();

	Boxer(SDL_Texture** tex_idle_ = t_test_idle);
	virtual ~Boxer();

	virtual State* pickAction();
	bool setState(State* next, bool interrupt = true, bool auto_delete = true);
	void render() override;
	void update() override;
};
