#include "boxer.h"
#include "cumt_common.h"
#include "gamemanager.h"
#include "shitrndr/src/shitrndr.h"
#include "states.h"

using namespace cumt;

Boxer::Boxer(SDL_Texture** tex_idle_)
{
	sd.parent = this;
	sd.tex_idle = tex_idle_;
	state = new IdleState(&sd);
}
Boxer::~Boxer()
{
	delete state;
}

bool Boxer::is_player() { return this==GM::player; }
void Boxer::takeDamage(float dmg)
{
	health -= dmg;
	if(health<=0)
		knockOut();
}
void Boxer::knockOut()
{
	GM::finishRound(this);
}

static State* to_delete = nullptr;
bool Boxer::setState(State *next, bool interrupt, bool auto_delete)
{
	if(interrupt && !state->interruptable)
	{
		if(auto_delete)
			delete next;
		return false;
	}
	if(interrupt) state->interrupt();

	state->exit(next);
	to_delete = state;
	state = next;
	state->enter();

	return true;
}

State* Boxer::pickAction()
{
	if(common::frand()>.5)
		return new WindupState(&sd);
	return new IdleState(&sd);
}

void Boxer::update()
{
	if(to_delete)
	{
		delete to_delete;
		to_delete = nullptr;
	}
	state->update();
	state->t += FD::delta;
	if(state->t>=state->dur)
	{
		setState(state->get_next(), false);
	}
}
void Boxer::render()
{
	using namespace shitrndr;
	Copy(state->tex[is_player()], getRect());
	SDL_Rect r = getRect();
	r.h /= 10;
	r.w *= health;
	SetColour({0,255,0,255});
	FillRect(r);
}
