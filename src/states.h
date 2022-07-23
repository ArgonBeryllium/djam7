#pragma once
#include <functional>
#include <SDL2/SDL.h>
#include "boxer.h"
#include "assets.h"

enum hitDir
{
	NONE = 0,
	LEFT = 1, RIGHT = 2,
	BOTH = 3
};
struct State
{
	std::function<State*()> get_next;
	float dur = 0.5, t = 0;
	bool interruptable = true;
	hitDir vuln = BOTH;
	hitDir dmg = NONE;
	float remaining() { return dur-t; }
	float completion() { return t/dur; }

	SDL_Texture** tex = t_debug_sided;
	Boxer* parent;

	State(Boxer* parent_) : parent(parent_) {}

	virtual void enter() {}
	virtual void update() {}
	virtual void exit() {}
	virtual void interrupt() {}
};
struct IdleState : State
{
	IdleState(Boxer* parent_) : State(parent_)
	{
		get_next = [this]() { return new IdleState(parent); };
		tex = t_test_idle;
		vuln = LEFT;
	}
};
struct HitState : State
{
	HitState(Boxer* parent_) : State(parent_)
	{
		get_next = [this]() { return new IdleState(parent); };
		tex = t_test_hit;
	}
};
struct PunchState : State
{
	bool hit = 0;
	float dmg_v;
	PunchState(Boxer* parent_, SDL_Texture** tex_ = t_test_punch, float dmg_v_ = .2, hitDir dir = LEFT, float dur_ = .3, hitDir vuln_ = NONE) :
		State(parent_), dmg_v(dmg_v_)
	{
		get_next = [this]() { return new IdleState(parent); };
		tex = tex_;
		dmg = dir;
		dur = dur_;
		vuln = vuln_;
		interruptable = false;
	}
	void update() override
	{
		if(hit) return;
		if(parent->opponent->state->vuln & dmg)
		{
			parent->opponent->setState(new HitState(parent->opponent));
			hit = true;
		}
	}
};
struct WindupState : State
{
	WindupState(Boxer* parent_, float dur_) : State(parent_)
	{
		dur = dur_;
		get_next = [this]() { return new PunchState(parent); };
		tex = t_test_windup;
	}
};

