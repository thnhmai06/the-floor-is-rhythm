#pragma once
#include <stdexcept>
#include <SDL3/SDL_error.h>

namespace SDL_Exception
{
	struct SDL_Exception : std::runtime_error
	{
		explicit SDL_Exception(const std::string& message) : std::runtime_error(message + '\n' + SDL_GetError()) {}
	};
	namespace SDL
	{
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
	namespace Shader
	{
		struct SDL_InvalidShaderStage : SDL_Exception
		{
			explicit SDL_InvalidShaderStage() : SDL_Exception("Invalid shader stage") {}
		};
		struct SDL_UnrecognizedBackendShaderFormat : SDL_Exception
		{
			explicit SDL_UnrecognizedBackendShaderFormat() : SDL_Exception("Unrecognized backend shader format") {}
		};
		struct SDL_FailedToLoadShaderFromDisk : SDL_Exception
		{
			explicit SDL_FailedToLoadShaderFromDisk(const std::string& fullPath) : SDL_Exception("Failed to load shader from disk: " + fullPath) {}
		};
		struct SDL_FailedToCreateShader : SDL_Exception
		{
			explicit SDL_FailedToCreateShader() : SDL_Exception("Failed to create shader") {}
		};
	}
}
