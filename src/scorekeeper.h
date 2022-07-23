#pragma once
struct ScoreKeeper
{
	static int score;
	static void init();
	static void punch();
	static void finishCombo();
	static void render();
	static int getStreak();
};
