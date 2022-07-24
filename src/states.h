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
	hitDir dir;
	HitState(StateData* data_, hitDir dir_ = std::rand()%2?LEFT:RIGHT) : State(data_), dir(dir_)
	{
		cp = data->cp_hits[dir];
		dur = data->dur_hit;
		if(!data->parent->is_player() && ScoreKeeper::getStreak())
			dur -= .05*ScoreKeeper::getStreak();
	}
	void enter() override
	{
		if(!data->parent->is_player())
			ScoreKeeper::punch(dir);
	}
	void exit(State* next) override
	{
		// TODO find a better way to compare state types
		if(!dynamic_cast<HitState*>(next) && !data->parent->is_player())
			ScoreKeeper::finishCombo();
	}
};
struct WinddownState : State
{
	WinddownState(StateData* data_, hitDir dir = LEFT) : State(data_)
	{
		dur = data->dur_winddowns[dir];
		cp = data->cp_winddowns[dir];
		vuln = dir;
	}
};
struct PunchState : State
{
	bool hit = 0;
	PunchState(StateData* data_, hitDir dir = LEFT) : State(data_)
	{
		get_next = [this, dir]() { return new WinddownState(data, dir); };
		vuln = NONE;
		dur = data->dur_punches[dir];
		dmg = dir;
		cp = data->cp_punches[dir];
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
	void exit(State* next) override
	{
		// was causing a bug in the final hour of the jam :)
		/*
		if(!hit && data->parent->is_player())
		{
			// takeDamage may call setState, which calls this function, so I'm reusing hit as a loop guard
			hit = true;
			data->parent->takeDamage(.5);
		}
		*/
	}
};
struct WindupState : State
{
	WindupState(StateData* data_, hitDir dir = LEFT) : State(data_)
	{
		get_next = [this, dir]() { return new PunchState(data, dir); };
		dur = data->dur_windups[dir];
		cp = data->cp_windups[dir];
	}
};

inline hitDir getCompliment(hitDir d)
{
	if(d==LEFT) return RIGHT;
	if(d==RIGHT) return LEFT;
	if(d==BACK) return NONE;
	return BACK;
}
struct DodgeState : State
{
	DodgeState(StateData* data_, const hitDir& dir) : State(data_)
	{
		vuln = getCompliment(dir);
		dur = data->dur_dodges[dir];
		cp = data->cp_dodges[dir];
	}
};

struct SwitchingState : State
{
	static SwitchingState* instance;
	SwitchingState(StateData* data_) : State(data_)
	{
		interruptable = false;
		cp = data->cp_idle;
		instance = this;
	}
	void enter() override { instance = this; }
	void exit(State* next) override { instance = nullptr; }
	void update() override
	{
		data->parent->pos = cumt::common::lerp(data->parent->pos, data->parent->is_player()?GM::spot_p:GM::spot_o, completion());
	}
};

struct StumbleState : State
{
	StumbleState(StateData* data_) : State(data_)
	{
		get_next = [this]() { return data->parent->getKnockOutResult()(); };
		interruptable = false;
		vuln = NONE;
		cp = data->cp_stumble;
		dur = 5;
	}

	void enter() override
	{
		GM::setFreeze();
	}
	void exit(State* next) override
	{
		GM::setFreeze(false);
	}
	void update() override
	{
		using namespace cumt;
		data->parent->pos = (data->parent->is_player()?GM::spot_p:GM::spot_o) + v2f(std::sin(FD::time*2)*.2*(1-completion()), 0);
	};
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
