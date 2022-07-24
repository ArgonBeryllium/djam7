#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cstddef>
#include <cumt_audio.h>
#include <cumt_logging.h>
#include <cumt_render.h>
#include <vector>

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

struct Clip
{
	std::vector<SDL_Texture*> frames;

	inline SDL_Texture* getFrame(float f)
	{
		return frames[std::min(frames.size()-1, std::max(size_t(0), size_t(std::round(f*frames.size()))))];
	}
};
struct ClipPair
{
	Clip* front, *back;
	Clip* operator[](const size_t& index) { return getClip(index); }
	Clip* getClip(const size_t& index) { return index?back:front; }
};

inline static Clip* loadClip(const char* path_start, size_t frames, size_t start_frame = 1, const char* extension = "png")
{
	Clip* out = new Clip;
	for(size_t i = start_frame; i != start_frame+frames; i++)
		out->frames.push_back(loadTex((std::string(path_start)+std::to_string(i)+"."+extension).c_str()));
	return out;
}
inline static ClipPair* loadClipPair(const char* path_start, size_t frames, size_t start_frame = 1, const char* extension = "png")
{
	ClipPair* out = new ClipPair;
	out->front = loadClip((std::string(path_start)+"_f_").c_str(), frames, start_frame, extension);
	out->back =  loadClip((std::string(path_start)+"_b_").c_str(), frames, start_frame, extension);
	return out;
}

inline Mix_Chunk* sfx_shoot;
inline SDL_Texture* t_debug;
inline ClipPair *cp_test_idle, *cp_test_hit, *cp_test_punch, *cp_test_windup, *cp_debug;
inline ClipPair *cp_test2_idle, *cp_test2_hit, *cp_test2_punch, *cp_test2_windup;

inline static void loadAssets()
{
	using namespace cumt;
	cumt::audio::init();
	cumt::RenderData::initTTF();

	sfx_shoot = loadSFX("res/test.wav");
	RenderData::loadFont("res/m6x11.ttf", 22);

	t_debug = loadTex("res/debug.png");
	Clip* debug_clip = new Clip();
	debug_clip->frames.push_back(t_debug);
	cp_debug = new ClipPair{debug_clip, debug_clip};

	cp_test_idle =   loadClipPair("res/test", 1, 1);
	cp_test_punch =  loadClipPair("res/test", 6, 7);
	cp_test_hit =    loadClipPair("res/test", 12, 18);
	cp_test_windup = loadClipPair("res/test", 7, 1);
	cp_test2_idle =   loadClipPair("res/test2", 1, 1);
	cp_test2_punch =  loadClipPair("res/test2", 6, 7);
	cp_test2_hit =    loadClipPair("res/test2", 12, 18);
	cp_test2_windup = loadClipPair("res/test2", 7, 1);
}
