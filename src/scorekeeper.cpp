#include "scorekeeper.h"
#include "boxer.h"
#include "cumt_render.h"
#include "gamemanager.h"
#include "shitrndr/src/shitrndr.h"
#include <string>

int ScoreKeeper::score = 0;
static int streak = 0;
static hitDir lastd = NONE;
void ScoreKeeper::init()
{
	score = 0;
	streak = 0;
}
void ScoreKeeper::finishCombo()
{
	lastd = NONE;
	if(streak%2 != 0)
		GM::prepSwap();
	streak = 0;
}
void ScoreKeeper::punch(hitDir dir)
{
	if(dir != lastd) score += 50;
	streak++;
	score += 100*streak;
}
void ScoreKeeper::render()
{
	using namespace cumt;
	if(streak)
		render::text(Thing2D::spaceToScr(GM::spot_p+v2f(.5+(streak%2?-.5:.5), streak*.1)), std::to_string(streak)+(streak%2?"!":" :)"));
	render::text(v2f(.5, 1./64)*shitrndr::WindowProps::getSize(), std::to_string(score), *td_c);
}
int ScoreKeeper::getStreak() { return streak; }
