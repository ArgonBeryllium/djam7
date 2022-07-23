#include <iostream>
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

struct GameScene : Scene
{
	Boxer* a, *b;
	inline Boxer* getP() { return GM::player; }
	inline Boxer* getO() { return GM::opponent; }
	void load() override
	{
		using namespace shitrndr;
		bg_col = {45,25,45,255};
		Thing2D::view_scale = 5;
		Thing2D::view_pos = {.5, .5};

		a = set.instantiate(new Boxer(), "boxer A");
		b = set.instantiate(new Boxer(), "boxer B");
		a->opponent = b;
		b->opponent = a;

		a->sd.dur_windup = .1;
		a->sd.dur_punch = .1;

		b->sd.tex_idle = t_test2_idle;
		b->sd.tex_punch = t_test2_punch;
		b->sd.tex_windup = t_test2_windup;
		b->sd.tex_hit = t_test2_hit;
		b->sd.dur_windup = .5;
		b->sd.dur_punch = .5;
		b->sd.dur_hit = .5;

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
		set.render();
		getP()->render();
		ScoreKeeper::render();
	}
	void onKey(SDL_Keycode k) override
	{
		switch(k)
		{
			case SDLK_z:
				// TODO find a better way to compare state types
				if(!dynamic_cast<WindupState*>(getP()->state))
					getP()->setState(new WindupState(&getP()->sd));
				break;
			case SDLK_r:
				unload();
				load();
				break;
			case SDLK_SPACE:
				a->setState(new SwitchingState(&a->sd), false);
				b->setState(new SwitchingState(&b->sd), false);
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
	quickInit(480, 540, ip);

	gameSetup();
	onLoop = gameLoop;
	onKey = gameKeyDown;
	start();

	return 0;
}
