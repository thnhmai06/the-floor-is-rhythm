#pragma once
#include <stdexcept>
#include <SDL3/SDL_error.h>

namespace SDL_Exception
{
	namespace Critical
	{
		struct SDL_Exception : std::runtime_error
		{
			explicit SDL_Exception(const std::string& message) : std::runtime_error(message + '\n' + SDL_GetError()) {}
		};

		struct SDL_InitSDL_Failed : SDL_Exception
		{
			explicit SDL_InitSDL_Failed() : SDL_Exception("Initializing SDL failed") {}
		};
		struct SDL_CreateWindow_Failed : SDL_Exception
		{
			explicit SDL_CreateWindow_Failed() : SDL_Exception("Creating window failed") {}
		};
		struct SDL_CreateGPUDevice_Failed : SDL_Exception
		{
			explicit SDL_CreateGPUDevice_Failed() : SDL_Exception("Creating GPU device failed") {}
		};
		struct SDL_ClaimWindowForGPUDevice_Failed : SDL_Exception
		{
			explicit SDL_ClaimWindowForGPUDevice_Failed() : SDL_Exception("Claiming window for GPU device failed") {}
		};
		struct SDL_AcquireGPUCommandBuffer_Failed : SDL_Exception
		{
			explicit SDL_AcquireGPUCommandBuffer_Failed() : SDL_Exception("Acquiring GPU command buffer failed") {}
		};
		struct SDL_SubmitGPUCommandBuffer_Failed : SDL_Exception
		{
			explicit SDL_SubmitGPUCommandBuffer_Failed() : SDL_Exception("Submitting GPU command buffer failed") {}
		};
	}
}
