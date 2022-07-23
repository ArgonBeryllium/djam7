#include <cstdlib>
#include <functional>
#include <iostream>
#include <cumt.h>
#include <cumt_common.h>
#include <cumt_render.h>
#include "assets.h"
#include "scenes.h"
using namespace cumt;

struct Player : Thing2D
{
	void update() override
	{
		pos += common::inVec()*FD::delta*10;
		pos.x = std::max(scrToSpace({}).x, std::min(scrToSpace({shitrndr::WindowProps::getWidth(), 0}).x, pos.x));
	}
	void render() override
	{
		using namespace shitrndr;
		SetColour({0,0,0,255});
		v2i p = spaceToScr(pos);
		FillCircle(p.x, p.y, scl.x/2*getScalar());
	}
	void shoot()
	{
		audio::play(sfx_shoot);
		auto* p = parent_set->instantiate(new Thing2D(pos, {.2, 1}));
		common::act(parent_set, [p]() { p->pos.y -= FD::delta*50; }, .3, [p](){ p->parent_set->scheduleDestroy(p); });
	}
};
Player* player;

struct S_Splash : Scene
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
		Scene::setActive(index+1);
	}
};
struct S_Game : Scene
{
	void start() override
	{
		player = set.instantiate(new Player());
	}
	void load() override
	{
		using namespace shitrndr;
		bg_col = {245,245,245,255};
		Thing2D::view_scale = .5;
	}
	void onKey(SDL_Keycode k) override
	{
		switch(k)
		{
			case SDLK_SPACE:
				player->shoot();
				break;
			default: break;
		}
	}
};

void gameSetup()
{
	using namespace shitrndr;
	WindowProps::setLockType(shitrndr::WindowProps::BARS);
	Scene::scenes = {new S_Splash, new S_Game};
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
