#pragma once
#include <SDL2/SDL_render.h>
#include <cumt_things.h>
#include "assets.h"

struct State;
struct Boxer;
enum hitDir
{
	NONE = 0,
	LEFT = 1, RIGHT = 2,
	BOTH = 3,
	BACK = 4
};
struct StateData
{
	Boxer* parent;
	float dur_idle_base = .5, dur_idle_range = 0;
	float dur_hit = .5;
	std::map<hitDir, float> dur_dodges = {{LEFT, .4}, {BACK, .2}, {RIGHT, .4}};
	std::map<hitDir, float> dur_punches = {{LEFT, .2}, {RIGHT, .2}};
	std::map<hitDir, float> dur_windups = {{LEFT, .3}, {RIGHT, .3}};

	float strength = .1;

	ClipPair* cp_idle    = cp_test_idle;
	ClipPair* cp_windup  = cp_test_windup;
	ClipPair* cp_hit     = cp_test_hit;
	ClipPair* cp_punch   = cp_test_punch;
	ClipPair* cp_victory = cp_debug;
	ClipPair* cp_loss    = cp_debug;
};
struct Boxer : cumt::Thing2D
{
	State* state;
	StateData sd;
	Boxer* opponent;
	bool is_player();

	float max_health = 1, health;
	void takeDamage(float dmg);
	virtual void knockOut();

	bool acting = false;
	bool act(State* next);
	bool setState(State* next, bool interrupt = true, bool auto_delete = true);
	void forceState(State* next, bool interrupt = true);

	Boxer(ClipPair* cp_idle_ = cp_test_idle);
	virtual ~Boxer();

	virtual State* pickAction();
	void render() override;
	void update() override;
};
