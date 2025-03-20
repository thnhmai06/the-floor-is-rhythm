#include "render/shader.h" // Header
#include <SDL3/SDL_filesystem.h>
#include "expections/sdl.h"

using namespace SDL_Exception::Shader;
constexpr char Shader_Location[] = "content/Shaders/Compiled/";
static const char* Base_Path {};

void init_asset_loader()
{
	Base_Path = SDL_GetBasePath();
}

SDL_GPUShader* load_shader(
	SDL_GPUDevice* device,
	const char* shader_filename,
	const Uint32 sampler_count,
	const Uint32 uniform_buffer_count,
	const Uint32 storage_buffer_count,
	const Uint32 storage_texture_count
) {
	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (SDL_strstr(shader_filename, ".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shader_filename, ".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		throw SDL_InvalidShaderStage();
	}

	char full_path[256];
	const SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(full_path, sizeof(full_path), "%s/%sSPIRV/%s.spv", Base_Path, Shader_Location, shader_filename);
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(full_path, sizeof(full_path), "%s/%sMSL/%s.msl", Base_Path, Shader_Location, shader_filename);
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main0";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
		SDL_snprintf(full_path, sizeof(full_path), "%s/%sDXIL/%s.dxil", Base_Path, Shader_Location, shader_filename);
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	}
	else throw SDL_UnrecognizedBackendShaderFormat();

	size_t codeSize;
	void* code = SDL_LoadFile(full_path, &codeSize);
	if (code == nullptr) throw SDL_FailedToLoadShaderFromDisk(full_path);

	SDL_GPUShaderCreateInfo shaderInfo {};
	shaderInfo.code = static_cast<const Uint8*>(code);
	shaderInfo.code_size = codeSize;
	shaderInfo.entrypoint = entrypoint;
	shaderInfo.format = format;
	shaderInfo.stage = stage;
	shaderInfo.num_samplers = sampler_count;
	shaderInfo.num_uniform_buffers = uniform_buffer_count;
	shaderInfo.num_storage_buffers = storage_buffer_count;
	shaderInfo.num_storage_textures = storage_texture_count;

	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == nullptr)
	{
		SDL_free(code);
		throw SDL_FailedToCreateShader();
	}

	SDL_free(code);
	return shader;
}
