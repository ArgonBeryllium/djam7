#include "scenes.h"
#include <cumt.h>

using namespace cumt;

std::vector<Scene*> Scene::scenes = {};
size_t Scene::active = 0;
size_t Scene::nactive = 0;

static float trans_timer = 0;
static bool is_trans = 1;

Scene* Scene::getActive() { return scenes[active]; }
void Scene::setActive(size_t i)
{
	nactive = i;
	is_trans = 1;
	trans_timer = 0;
}
void Scene::init()
{
	size_t i = 0;
	for(Scene* s : scenes)
	{
		s->index = i++;
		s->start();
	}
	getActive()->load();
}
void Scene::update()
{
	if(active==nactive)
	{
		if(is_trans)
		{
			trans_timer += FD::delta/getActive()->ti_dur;
			getActive()->transIn(trans_timer);
			if(trans_timer>=1)is_trans = 0;
		}
	}
	else
	{
		trans_timer += FD::delta/getActive()->to_dur;
		getActive()->transOut(trans_timer);
		if(trans_timer>=1)
		{
			getActive()->unload();
			active = nactive;
			getActive()->load();
			trans_timer = 0;
		}
	}
}
