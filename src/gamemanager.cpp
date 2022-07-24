#include "gamemanager.h"
#include "assets.h"
#include "shitrndr/src/shitrndr.h"
#include "states.h"
using namespace cumt;

Boxer* GM::player, *GM::opponent;
v2f GM::spot_p = {0,.4}, GM::spot_o = {0, 0};
static Boxer* op;

static bool swap = false, is_finished = false, freeze = false;
static Boxer* loser;

void GM::init(Boxer *p, Boxer *o)
{
	swap = false;
	is_finished = false;
	freeze = false;

	op = player = p;
	opponent = o;
	p->pos = spot_p;
	o->pos = spot_o;
}

bool GM::finished() { return is_finished; }
bool GM::playerWon() { return player != loser && player == op; }
bool GM::getFreeze() { return freeze; }
void GM::setFreeze(bool f) { freeze = f; }

void GM::prepSwap()
{
	swap = true;
}
void GM::update()
{
	if(swap)
	{
		player->forceState(new SwitchingState(&player->sd));
		opponent->forceState(new SwitchingState(&opponent->sd));
		std::swap(player, opponent);
		swap = false;
		setFreeze(false);
	}
}
void GM::render()
{
	if(is_finished)
	{
		setFreeze();
		SDL_Rect r = shitrndr::WindowProps::getSizeRect();
		if(playerWon())
			render::text(shitrndr::WindowProps::getSize()/2, "You Win!", *td_ca);
		else
			render::text(shitrndr::WindowProps::getSize()/2, "You Lost.", *td_ca);
		render::text(v2f(.5, .6)*shitrndr::WindowProps::getSize(), "[R] to RESTART", *td_ca);
		render::text(v2f(.5, .65)*shitrndr::WindowProps::getSize(), "[Q] or [ESC] to QUIT", *td_ca);
	}
}
void GM::finishRound(Boxer *loser_)
{
	is_finished = true;
	setFreeze();
	loser = loser_;
	loser->forceState(new LossState(&loser->sd));
	loser->opponent->forceState(new VictoryState(&loser->opponent->sd));
}
