#pragma once
#include <atomic>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "engine/audio/mixer.h"

inline bool DEBUG = false;
inline SDL_Window* window = nullptr;
inline SDL_Renderer* renderer = nullptr;
inline std::unique_ptr<Engine::Audio::Mixer> mixer;
inline std::atomic is_running = true;