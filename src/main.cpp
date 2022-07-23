#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <cumt.h>
#include <cumt_common.h>
#include <cumt_render.h>
#include "assets.h"
#include "cumt_things.h"
#include "scenes.h"
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
		if(getActive()==this)
			Scene::setActive(index+1);
	}
};

struct Boxer;
enum hitDir
{
	NONE = 0,
	LEFT = 1, RIGHT = 2,
	BOTH = 3
};
struct State
{
	std::function<State*()> get_next;
	float dur = 0.5, t = 0;
	bool interruptable = true;
	hitDir vuln = BOTH;
	hitDir dmg = NONE;
	float remaining() { return dur-t; }
	float completion() { return t/dur; }

	SDL_Texture** tex = t_debug_sided;
	Boxer* parent;

	State(Boxer* parent_) : parent(parent_) {}

	virtual void enter() {}
	virtual void update() {}
	virtual void exit() {}
	virtual void interrupt() {}
};
struct IdleState : State
{
	IdleState(Boxer* parent_) : State(parent_)
	{
		get_next = [this]() { return new IdleState(parent); };
		tex = t_test_idle;
		vuln = LEFT;
	}
};
struct Boxer : Thing2D
{
	State* state = new IdleState(this);
	Boxer* opponent;
	bool is_player;

	Boxer(bool is_player_ = false) :
		Thing2D(is_player_?v2f(0,.1):v2f(0,-.1)), is_player(is_player_) {}

	bool setState(State* next, bool interrupt = true, bool auto_delete = true)
	{
		if(interrupt && !state->interruptable)
		{
			if(auto_delete)
				delete next;
			return false;
		}
		if(interrupt) state->interrupt();

		state->exit();
		delete state;
		state = next;
		state->enter();

		return true;
	}
	void render() override
	{
		using namespace shitrndr;
		Copy(state->tex[is_player], getRect());
	}
	void update() override
	{
		state->update();
		state->t += FD::delta;
		if(state->t>=state->dur)
		{
			setState(state->get_next(), false);
		}
	}
};
struct HitState : State
{
	HitState(Boxer* parent_) : State(parent_)
	{
		get_next = [this]() { return new IdleState(parent); };
		tex = t_test_hit;
	}
};
struct PunchState : State
{
	bool hit = 0;
	float dmg_v = .2;
	PunchState(Boxer* parent_) : State(parent_)
	{
		get_next = [this]() { return new IdleState(parent); };
		dur = .2;
		tex = t_test_punch;
		dmg = LEFT;
		vuln = NONE;
		interruptable = false;
	}
	void update() override
	{
		if(hit) return;
		if(parent->opponent->state->vuln & dmg)
		{
			parent->opponent->setState(new HitState(parent->opponent));
			hit = true;
		}
	}
};
struct WindupState : State
{
	WindupState(Boxer* parent_, float dur_) : State(parent_)
	{
		dur = dur_;
		get_next = [this]() { return new PunchState(parent); };
		tex = t_test_windup;
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
	}
	void load() override
	{
		using namespace shitrndr;
		bg_col = {45,25,45,255};
		Thing2D::view_scale = 4;
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
			case SDLK_SPACE:
				getP()->setState(new WindupState(getP(), .2));
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
