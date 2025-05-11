#pragma once
#include <atomic>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "structures/audio/mixer.h"

inline bool DEBUG = false;
inline SDL_Window* window = nullptr;
inline SDL_Renderer* renderer = nullptr;
inline std::unique_ptr<Structures::Audio::Mixer> mixer;
inline std::atomic is_running = true;