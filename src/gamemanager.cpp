#include "gamemanager.h"
#include "states.h"
using namespace cumt;

Boxer* GM::player, *GM::opponent;
v2f GM::spot_p = {0,.2}, GM::spot_o = {0, -.2};
void GM::init(Boxer *p, Boxer *o)
{
	player = p;
	opponent = o;
	p->pos = spot_p;
	o->pos = spot_o;
}
static bool swap = false;
void GM::prepSwap()
{
	swap = true;
}
void GM::update()
{
	if(swap)
	{
		player->setState(new SwitchingState(&player->sd), false);
		opponent->setState(new SwitchingState(&opponent->sd), false);
		std::swap(player, opponent);
		swap = false;
	}
}
