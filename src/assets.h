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

inline SDL_Texture* t_debug, *t_bg;
inline Clip *c_bgr;
inline ClipPair *cp_test_idle, *cp_test_hit, *cp_test_punch, *cp_test_windup, *cp_debug;
inline ClipPair *cp_test2_idle, *cp_test2_hit, *cp_test2_punch, *cp_test2_windup;
inline ClipPair *cp_p_idle, *cp_p_hit_r, *cp_p_hit_l, *cp_p_winddown_l, *cp_p_winddown_r, *cp_p_windup_l, *cp_p_windup_r, *cp_p_punch_l, *cp_p_punch_r, *cp_p_dodge_l, *cp_p_dodge_b, *cp_p_dodge_r, *cp_p_win, *cp_p_stumble, *cp_p_lose;
inline ClipPair *cp_o_idle, *cp_o_hit_r, *cp_o_hit_l, *cp_o_winddown_l, *cp_o_winddown_r, *cp_o_windup_l, *cp_o_windup_r, *cp_o_punch_l, *cp_o_punch_r, *cp_o_dodge_l, *cp_o_dodge_b, *cp_o_dodge_r, *cp_o_win, *cp_o_stumble, *cp_o_lose;
inline cumt::render::TextData* td_c, *td_ca;
inline const SDL_Colour COL_BG = {04, 23, 37, 255};

inline static void loadAssets()
{
	using namespace cumt;
	cumt::RenderData::initTTF();

	RenderData::loadFont("res/m6x11.ttf", 14);

	td_c = new render::TextData;
	td_c->anchor = render::TextData::CENTRE;
	td_ca = new render::TextData;
	td_ca->anchor = render::TextData::CENTRE;
	td_ca->col = COL_BG;

	t_bg = loadTex("res/bg_rotate_1.png");
	t_debug = loadTex("res/debug.png");
	Clip* debug_clip = new Clip();
	debug_clip->frames.push_back(t_debug);
	cp_debug = new ClipPair{debug_clip, debug_clip};

	c_bgr = loadClip("res/bg_rotate_", 10);

	cp_test_idle =   loadClipPair("res/test", 1, 1);
	cp_test_punch =  loadClipPair("res/test", 6, 7);
	cp_test_hit =    loadClipPair("res/test", 12, 18);
	cp_test_windup = loadClipPair("res/test", 7, 1);
	cp_test2_idle =   loadClipPair("res/test2", 1, 1);
	cp_test2_punch =  loadClipPair("res/test2", 6, 7);
	cp_test2_hit =    loadClipPair("res/test2", 12, 18);
	cp_test2_windup = loadClipPair("res/test2", 7, 1);

	cp_p_idle = loadClipPair("res/player", 9);

	cp_p_windup_l   = loadClipPair("res/player", 6, 10);
	cp_p_punch_l    = loadClipPair("res/player", 6, 16);
	cp_p_winddown_l = loadClipPair("res/player", 5, 22);

	cp_p_windup_r   = loadClipPair("res/player", 6, 27);
	cp_p_punch_r    = loadClipPair("res/player", 6, 33);
	cp_p_winddown_r = loadClipPair("res/player", 5, 39);

	cp_p_dodge_l = loadClipPair("res/player", 12, 44);
	cp_p_dodge_r = loadClipPair("res/player", 11, 56);
	cp_p_dodge_b = loadClipPair("res/player", 11, 65);
	cp_p_hit_r   = loadClipPair("res/player", 15, 78);
	cp_p_hit_l   = loadClipPair("res/player", 15, 93);

	cp_p_stumble  = loadClipPair("res/player", 41, 109);
	cp_p_lose    = loadClipPair("res/player", 14, 150);
	cp_p_win     = loadClipPair("res/player", 17, 165);

	cp_o_idle = loadClipPair("res/opp", 9);

	cp_o_windup_l   = loadClipPair("res/opp", 6, 10);
	cp_o_punch_l    = loadClipPair("res/opp", 6, 16);
	cp_o_winddown_l = loadClipPair("res/opp", 5, 22);

	cp_o_windup_r   = loadClipPair("res/opp", 6, 27);
	cp_o_punch_r    = loadClipPair("res/opp", 6, 33);
	cp_o_winddown_r = loadClipPair("res/opp", 5, 39);

	cp_o_dodge_l = loadClipPair("res/opp", 12, 44);
	cp_o_dodge_r = loadClipPair("res/opp", 11, 56);
	cp_o_dodge_b = loadClipPair("res/opp", 11, 65);
	cp_o_hit_r   = loadClipPair("res/opp", 15, 78);
	cp_o_hit_l   = loadClipPair("res/opp", 15, 93);

	cp_o_stumble  = loadClipPair("res/opp", 41, 109);
	cp_o_lose    = loadClipPair("res/opp", 14, 150);
	cp_o_win     = loadClipPair("res/opp", 17, 165);
}
