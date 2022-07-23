#include <iostream>
#include <cumt.h>
#include "assets.h"
#include "scenes.h"
#include "boxer.h"
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
	inline Boxer* getP() { return a->is_player?a:b; }
	void start() override
	{
		a = set.instantiate(new Boxer(true), "boxer A");
		b = set.instantiate(new Boxer(false), "boxer B");
		a->opponent = b;
		b->opponent = a;
		b->sd.tex_idle = t_test2_idle;
		b->sd.tex_punch = t_test2_punch;
		b->sd.tex_windup = t_test2_windup;
		b->sd.tex_hit = t_test2_hit;
	}
	void load() override
	{
		using namespace shitrndr;
		bg_col = {45,25,45,255};
		Thing2D::view_scale = 5;
		Thing2D::view_pos = {.5, .5};
	}
	void loop() override
	{
		set.update();
		set.render();
		getP()->render();
	}
	void onKey(SDL_Keycode k) override
	{
		switch(k)
		{
			case SDLK_z:
				getP()->setState(new WindupState(&getP()->sd));
				break;
			case SDLK_SPACE:
				a->setState(new SwitchingState(&a->sd));
				b->setState(new SwitchingState(&b->sd));
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
