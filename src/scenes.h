#pragma once
#include <cumt_things.h>
#include <vector>

struct Scene
{
	static std::vector<Scene*> scenes;
	static size_t active, nactive;
	static void setActive(size_t i);
	static Scene* getActive();
	size_t index;

	static void init();
	static void update();

	float ti_dur = .5, to_dur = .5;

	cumt::ThingSet set;

	virtual ~Scene() {}

	virtual void transOut(float t)
	{
		using namespace shitrndr;
		SetColour({5,5,5,255});
		FillRect({0,0,WindowProps::getWidth(), int(WindowProps::getHeight()*t)});
	}
	virtual void transIn(float t)
	{
		transOut(1-t);
	}

	virtual void start() {}
	virtual void load() {}
	virtual void unload() {}

	virtual void loop()
	{
		set.update();
		set.render();
	}
	virtual void onKey(SDL_Keycode key) {}
	virtual void onKeyUp(SDL_Keycode key) {}
	virtual void onKeyHeld(SDL_Keycode key) {}
	virtual void onMB(size_t b) {}
	virtual void onMBUp(size_t b) {}
};
