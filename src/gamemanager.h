#pragma once
#include "boxer.h"
struct GM
{
	static cumt::v2f spot_p, spot_o;
	static Boxer* player, *opponent;
	static void init(Boxer* p, Boxer* o);
	static void prepSwap();
	static void update();
};
