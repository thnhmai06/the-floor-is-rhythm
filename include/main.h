#pragma once
#include <string>
#include <atomic>
#include <SDL3/SDL_filesystem.h>

inline const std::string Base_Path = SDL_GetBasePath();
inline std::atomic is_running = true;