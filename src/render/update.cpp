#include "render/update.h" // Header
#include <SDL3/SDL_gpu.h>
#include "main.h"
#include "expections/sdl.h"
#include "logging.h"

void update_screen(SDL_GPUDevice*& device, SDL_Window*& window)
{
    // create update command
    SDL_GPUCommandBuffer* command_buffer{ SDL_AcquireGPUCommandBuffer(device) };
	if (!command_buffer) THROW_CRITICAL(SDL_Exceptions::SDL::SDL_AcquireGPUCommandBuffer_Failed());

    // render everything here
    SDL_GPUTexture* swapchains_texture;
    SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchains_texture, nullptr, nullptr);
    if (swapchains_texture)
    {
        /*SDL_GPUColorTargetInfo color_target{};
        color_target.texture = swapchains_texture;
        color_target.store_op = SDL_GPU_STOREOP_STORE;
        color_target.load_op = SDL_GPU_LOADOP_CLEAR;
        color_target.clear_color = { 1.0f, 0.0f, 0.0f, 1.0f };

        std::vector color_targets{ color_target };
        SDL_GPURenderPass* render_pass
        {
            SDL_BeginGPURenderPass(command_buffer, color_targets.data(), color_targets.size(), nullptr)
        };
        SDL_EndGPURenderPass(render_pass);*/
    }

    // submit command
    if (!SDL_SubmitGPUCommandBuffer(command_buffer)) THROW_CRITICAL(SDL_Exceptions::SDL::SDL_SubmitGPUCommandBuffer_Failed());
}