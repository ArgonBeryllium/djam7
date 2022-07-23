#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cumt_audio.h>
#include <cumt_logging.h>
#include <cumt_render.h>

inline static Mix_Chunk* loadSFX(const char* path)
{
	Mix_Chunk* out = Mix_LoadWAV(path);
	if(!out) cumt::err("Failed to load SFX file '", path, "': ", Mix_GetError());
	return out;
}
inline static Mix_Music* loadMus(const char* path)
{
	Mix_Music* out = Mix_LoadMUS(path);
	if(!out) cumt::err("Failed to load music file '", path, "': ", Mix_GetError());
	return out;
}
inline static SDL_Texture* loadTex(const char* path)
{
	SDL_Texture* out = IMG_LoadTexture(shitrndr::ren, path);
	if(!out) cumt::err("Failed to load texture file '", path, "': ", IMG_GetError());
	return out;
}

inline Mix_Chunk* sfx_shoot;
inline SDL_Texture *t_test_idle[2], *t_test_hit[2], *t_test_punch[2], *t_test_windup[2], *t_debug, *t_debug_sided[2];

inline static void loadAssets()
{
	using namespace cumt;
	cumt::audio::init();
	cumt::RenderData::initTTF();

	sfx_shoot = loadSFX("res/test.wav");
	RenderData::loadFont("res/m6x11.ttf", 22);

	t_test_idle[0] = loadTex("res/test_f_idle.png");
	t_test_idle[1] = loadTex("res/test_b_idle.png");
	t_test_punch[0] = loadTex("res/test_f_punch.png");
	t_test_punch[1] = loadTex("res/test_b_punch.png");
	t_test_hit[0] = loadTex("res/test_f_hit.png");
	t_test_hit[1] = loadTex("res/test_b_hit.png");
	t_test_windup[0] = loadTex("res/test_f_windup.png");
	t_test_windup[1] = loadTex("res/test_b_windup.png");
	t_debug = loadTex("res/debug.png");
	t_debug_sided[0] = t_debug_sided[1] = t_debug;
}
