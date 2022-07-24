#pragma once
#include "boxer.h"
struct ScoreKeeper
{
	static int score;
	static void init();
	static void punch(hitDir d);
	static void finishCombo();
	static void render();
	static int getStreak();
};
