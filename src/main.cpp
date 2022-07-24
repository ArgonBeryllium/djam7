#include <cumt.h>
#include "assets.h"
#include "gamemanager.h"
#include "scenes.h"
#include "boxer.h"
#include "scorekeeper.h"
#include "states.h"

using namespace cumt;

struct SplashScene : Scene
{
	void load() override
	{
		using namespace shitrndr;
		bg_col = {25,15,35,255};
		common::act(&set, [](){}, 3, [this](){ Scene::setActive(this->index+1); });
	}
	void loop() override
	{
		set.update();
		render::text(shitrndr::WindowProps::getSize()/2+v2i(std::cos(FD::time*14.1)*5-50, std::sin(FD::time*3)*15), "ArBe game :)");
	}
	void onKey(SDL_Keycode k) override
	{
		if(nactive==index)
			Scene::setActive(index+1);
	}
};

struct Player : Boxer
{
	Player()
	{
		sd.cp_idle = cp_p_idle;
		sd.cp_hits = {{LEFT, cp_p_hit_l}, {RIGHT, cp_p_hit_r}};
		sd.cp_punches = {{LEFT, cp_p_punch_l}, {RIGHT, cp_p_punch_r}};
		sd.cp_windups = {{LEFT, cp_p_windup_l}, {RIGHT, cp_p_windup_r}};
		sd.cp_winddowns = {{LEFT, cp_p_winddown_l}, {RIGHT, cp_p_winddown_r}};
		sd.cp_dodges = {{LEFT, cp_p_dodge_l}, {BACK, cp_p_dodge_b}, {RIGHT, cp_p_dodge_r}};
		sd.cp_stumble = cp_p_stumble;
		sd.cp_loss = cp_p_lose;
		sd.cp_victory = cp_p_win;
	}
};

struct GameScene : Scene
{
	Boxer* a, *b;
	inline Boxer* getP() { return GM::player; }
	inline Boxer* getO() { return GM::opponent; }
	void load() override
	{
		using namespace shitrndr;
		bg_col = {45,25,45,255};
		Thing2D::view_scale = 8;
		Thing2D::view_pos = {.5, .5};

		a = set.instantiate(new Player(), "boxer A");
		b = set.instantiate(new Boxer(cp_test2_idle), "boxer B");
		a->opponent = b;
		b->opponent = a;

		a->sd.dur_windups[LEFT] = .2;
		a->sd.dur_punches[LEFT] = .1;

		b->sd.cp_idle = cp_test2_idle;
		b->sd.dur_windups[LEFT] = .5;
		b->sd.dur_punches[LEFT] = .2;
		b->sd.dur_hit = .7;

		GM::init(a, b);
		ScoreKeeper::init();
	}
	void unload() override
	{
		set.destroy(a);
		set.destroy(b);
	}
	void loop() override
	{
		GM::update();
		set.update();

		using namespace shitrndr;
		Copy(t_bg, WindowProps::getSizeRect());
		if(SwitchingState::instance)
			Copy(c_bgr->getFrame(SwitchingState::instance->completion()), WindowProps::getSizeRect());
		set.render();
		getP()->render();
		ScoreKeeper::render();
		GM::render();
	}
	void onKey(SDL_Keycode k) override
	{
		switch(k)
		{
			case SDLK_p:
				getP()->takeDamage(getP()->health);
				break;
			case SDLK_o:
				getO()->takeDamage(getO()->health);
				break;
			case SDLK_r:
				unload();
				load();
				break;
			case SDLK_SPACE:
				GM::prepSwap();
				break;
			default: break;
		}
		if(GM::finished()) return;
		// TODO find a better way to compare state types
		//if(!dynamic_cast<IdleState*>(getP()->state)) return;
		switch(k)
		{
			case SDLK_z:
					getP()->act(new WindupState(&getP()->sd, LEFT));
				break;
			case SDLK_x:
					getP()->act(new WindupState(&getP()->sd, RIGHT));
				break;
			case SDLK_LEFT:
			case SDLK_a:
				getP()->act(new DodgeState(&getP()->sd, LEFT));
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				getP()->act(new DodgeState(&getP()->sd, RIGHT));
				break;
			case SDLK_DOWN:
			case SDLK_s:
				getP()->act(new DodgeState(&getP()->sd, BACK));
				break;
			default: break;
		}
	}
};

void gameSetup()
{
	using namespace shitrndr;
	WindowProps::setLockType(shitrndr::WindowProps::BARS);
	Scene::scenes = {new SplashScene, new GameScene};
	Scene::init();

	loadAssets();
}
void gameLoop()
{
	Scene::getActive()->loop();
	Scene::update();
}
void gameKeyDown(const SDL_Keycode& k)
{
	switch (k)
	{
		case SDLK_q:
		case SDLK_ESCAPE:
			SDL_Quit();
			std::exit(0);
			break;
		default:
			Scene::getActive()->onKey(k);
			break;
	}
}

int main()
{
	InitParams ip;
	ip.sr_ps = 1;
	quickInit(720, 540, ip);

	gameSetup();
	onLoop = gameLoop;
	onKey = gameKeyDown;
	start();

	return 0;
}
