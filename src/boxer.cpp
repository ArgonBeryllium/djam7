#include "boxer.h"
#include "gamemanager.h"
#include "states.h"
#include <optional>

using namespace cumt;

Boxer::Boxer()
{
	sd.parent = this;
	state = new IdleState(&sd);
}
Boxer::~Boxer()
{
	delete state;
}

bool Boxer::is_player() { return this==GM::player; }

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
	if(is_player())
	{
		SetColour({255,0,0,255});
		auto c = spaceToScr(centre());
		FillCircle(c.x, c.y, 3);
	}
}
