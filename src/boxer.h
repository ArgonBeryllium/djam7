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

	SDL_Texture** tex_idle    = t_test_idle;
	SDL_Texture** tex_windup  = t_test_windup;
	SDL_Texture** tex_hit     = t_test_hit;
	SDL_Texture** tex_punch   = t_test_punch;
	SDL_Texture** tex_victory = t_debug_sided;
	SDL_Texture** tex_loss    = t_debug_sided;
};
struct Boxer : cumt::Thing2D
{
	State* state;
	StateData sd;
	Boxer* opponent;
	bool is_player();

	float health = 1;
	void takeDamage(float dmg);
	virtual void knockOut();

	Boxer(SDL_Texture** tex_idle_ = t_test_idle);
	virtual ~Boxer();

	virtual State* pickAction();
	bool setState(State* next, bool interrupt = true, bool auto_delete = true);
	void render() override;
	void update() override;
};
