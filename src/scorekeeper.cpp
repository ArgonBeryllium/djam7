#include "scorekeeper.h"
#include "cumt_render.h"
#include "gamemanager.h"

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
		render::text({0, 0}, std::to_string(streak));
}
int ScoreKeeper::getStreak() { return streak; }
