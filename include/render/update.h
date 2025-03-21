#pragma once
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_render.h>

void update_screen(SDL_GPUDevice*& device, SDL_Window*& window, SDL_Renderer* renderer);
