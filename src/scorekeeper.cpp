#include "scorekeeper.h"
#include "cumt_render.h"
#include "gamemanager.h"
#include <string>

int ScoreKeeper::score = 0;
static int streak = 0;
void ScoreKeeper::init()
{
	score = 0;
	streak = 0;
}
void ScoreKeeper::finishCombo()
{
	if(streak%2 != 0)
		GM::prepSwap();
	streak = 0;
}
void ScoreKeeper::punch()
{
	streak++;
	score += 100*streak;
}
void ScoreKeeper::render()
{
	using namespace cumt;
	if(streak)
		render::text(Thing2D::spaceToScr(GM::spot_p+v2f(.5+(streak%2?-.5:.5), streak*.1)), std::to_string(streak)+(streak%2?"!":" :)"));
	render::text({0, 0}, std::to_string(score));
}
int ScoreKeeper::getStreak() { return streak; }
