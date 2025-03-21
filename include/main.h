#pragma once
#include <string>
#include <atomic>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_video.h>

inline const std::string Base_Path = SDL_GetBasePath();
inline std::atomic<bool> running = true;
inline std::atomic<SDL_Window*> window = nullptr;