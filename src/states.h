#pragma once
#include <functional>
#include <SDL2/SDL.h>
#include "boxer.h"
#include "assets.h"
#include "cumt_common.h"

enum hitDir
{
	NONE = 0,
	LEFT = 1, RIGHT = 2,
	BOTH = 3
};

struct State
{
	StateData* data;
	std::function<State*()> get_next;

	bool interruptable = true;
	hitDir vuln = BOTH;
	hitDir dmg = NONE;

	float dur = 0.5;
	float t = 0;
	float remaining() { return dur-t; }
	float completion() { return t/dur; }

	SDL_Texture** tex = t_debug_sided;

	State(StateData* data_) : data(data_) {}

	virtual void enter() {}
	virtual void update() {}
	virtual void exit() {}
	virtual void interrupt() {}
};
struct IdleState : State
{
	IdleState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return new IdleState(data); };
		tex = data->tex_idle;
		dur = data->dur_idle_base + cumt::common::frand()*data->dur_idle_range;
		vuln = LEFT;
	}
};
struct HitState : State
{
	HitState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return new IdleState(data); };
		tex = data->tex_hit;
		dur = data->dur_hit;
	}
};
struct PunchState : State
{
	bool hit = 0;
	PunchState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return new IdleState(data); };
		interruptable = false;
		dur = data->dur_punch;
		tex = data->tex_punch;
	}
	void update() override
	{
		if(hit) return;
		if(data->parent->opponent->state->vuln & dmg)
		{
			data->parent->opponent->setState(new HitState(&data->parent->opponent->sd));
			hit = true;
		}
	}
};
struct WindupState : State
{
	WindupState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return new PunchState(data); };
		dur = data->dur_windup;
		tex = data->tex_windup;
	}
};

struct SwitchingState : State
{
	SwitchingState(StateData* data_) : State(data_)
	{
		get_next = [this](){ return new IdleState(data); };
		interruptable = false;
		tex = t_test_idle;
	}
	void update() override
	{
		data->parent->pos = cumt::common::lerp(data->parent->pos, cumt::v2f(0,1-2*data->parent->is_player)*.1, completion());
	}
	void exit() override
	{
		data->parent->is_player = !data->parent->is_player;
	}
};
