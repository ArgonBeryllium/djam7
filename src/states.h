#pragma once
#include <functional>
#include <SDL2/SDL.h>
#include "boxer.h"
#include "assets.h"
#include "cumt_common.h"
#include "gamemanager.h"
#include "scorekeeper.h"

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

	ClipPair* cp = cp_debug;

	State(StateData* data_);

	virtual void enter() {}
	virtual void update() {}
	virtual void exit(State* next) {}
	virtual void interrupt() {}
};

struct IdleState : State
{
	IdleState(StateData* data_);
	void enter() override
	{
		data->parent->acting = false;
	}
};

struct HitState : State
{
	HitState(StateData* data_) : State(data_)
	{
		cp = data->cp_hit;
		dur = data->dur_hit;
		if(!data->parent->is_player() && ScoreKeeper::getStreak())
			dur -= .05*ScoreKeeper::getStreak();
	}
	void enter() override
	{
		if(!data->parent->is_player())
			ScoreKeeper::punch();
	}
	void exit(State* next) override
	{
		// TODO find a better way to compare state types
		if(!dynamic_cast<HitState*>(next) && !data->parent->is_player())
			ScoreKeeper::finishCombo();
	}
};
struct PunchState : State
{
	bool hit = 0;
	PunchState(StateData* data_, hitDir dir = LEFT) : State(data_)
	{
		vuln = NONE;
		dur = data->dur_punches[dir];
		dmg = dir;
		cp = data->cp_punch;
	}
	void update() override
	{
		if(hit) return;
		if(data->parent->opponent->state->vuln & dmg)
		{
			data->parent->opponent->setState(new HitState(&data->parent->opponent->sd));
			data->parent->opponent->takeDamage(data->strength);
			hit = true;
		}
	}
};
struct WindupState : State
{
	WindupState(StateData* data_, hitDir dir = LEFT) : State(data_)
	{
		get_next = [this, dir]() { return new PunchState(data, dir); };
		dur = data->dur_windups[dir];
		cp = data->cp_windup;
	}
};

struct DodgeState : State
{
	DodgeState(StateData* data_, const hitDir& dir) : State(data_)
	{
		vuln = dir;
		dur = data->dur_dodges[dir];
		cp = cp_debug;
	}
};

struct SwitchingState : State
{
	SwitchingState(StateData* data_) : State(data_)
	{
		interruptable = false;
		cp = data->cp_idle;
	}
	void update() override
	{
		data->parent->pos = cumt::common::lerp(data->parent->pos, data->parent->is_player()?GM::spot_p:GM::spot_o, completion());
	}
};

struct VictoryState : State
{
	VictoryState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return new VictoryState(data); };
		cp = data->cp_victory;
		interruptable = false;
	}
};
struct LossState : State
{
	LossState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return new LossState(data); };
		cp = data->cp_loss;
		interruptable = false;
	}
};
