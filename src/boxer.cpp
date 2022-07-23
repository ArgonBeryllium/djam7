#include "boxer.h"
#include "states.h"
#include <optional>

using namespace cumt;

Boxer::Boxer(bool is_player_) :
	Thing2D(is_player_?cumt::v2f(0,.1):cumt::v2f(0,-.1)), is_player(is_player_)
{
	sd.parent = this;
	state = new IdleState(&sd);
}
Boxer::~Boxer()
{
	delete state;
}

bool Boxer::setState(State *next, bool interrupt, bool auto_delete)
{
	if(interrupt && !state->interruptable)
	{
		if(auto_delete)
			delete next;
		return false;
	}
	if(interrupt) state->interrupt();

	state->exit();
	delete state;
	state = next;
	state->enter();

	return true;
}

void Boxer::render()
{
	using namespace shitrndr;
	Copy(state->tex[is_player], getRect());
	if(is_player)
	{
		SetColour({255,0,0,255});
		auto c = spaceToScr(centre());
		FillCircle(c.x, c.y, 3);
	}
}
void Boxer::update()
{
	state->update();
	state->t += FD::delta;
	if(state->t>=state->dur)
	{
		setState(state->get_next(), false);
	}
}
