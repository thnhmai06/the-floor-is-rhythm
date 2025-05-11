#pragma once
#include <atomic>
#include <SDL3/SDL_video.h>

inline SDL_Window* window = nullptr;
inline std::atomic is_running = true;