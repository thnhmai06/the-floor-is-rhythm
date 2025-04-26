#pragma once
#include <string>
#include <atomic>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_video.h>

inline const std::string base_path = SDL_GetBasePath();
inline SDL_Window* window = nullptr;
inline std::atomic is_running = true;