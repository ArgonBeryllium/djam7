#pragma once
#include "boxer.h"
struct GM
{
	static cumt::v2f spot_p, spot_o;
	static Boxer* player, *opponent;
	static void init(Boxer* p, Boxer* o);

	static void prepSwap();

	static void finishRound(Boxer* loser);
	static bool finished();
	static bool playerWon();

	static bool getFreeze();
	static void setFreeze(bool f = true);

	static void update();
	static void render();
};
