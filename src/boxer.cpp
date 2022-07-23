#include "boxer.h"
#include "states.h"

using namespace cumt;

Boxer::Boxer(bool is_player_) :
	Thing2D(is_player_?cumt::v2f(0,.1):cumt::v2f(0,-.1)), is_player(is_player_)
{
	state = new IdleState(this);
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
