#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
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

inline static void loadAssets()
{
	using namespace cumt;
	cumt::audio::init();
	cumt::RenderData::initTTF();

	sfx_shoot = loadSFX("res/test.wav");
	RenderData::loadFont("res/m6x11.ttf", 22);
}
