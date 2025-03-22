#pragma once
#include <cstdint>
#include <SDL3/SDL_audio.h>

namespace Config
{
	namespace Default
	{
		namespace Audio
		{
			constexpr int32_t MAX_CHANNELS = 8;                // Số kênh phát hiệu ứng tối đa
			constexpr int32_t SAMPLE_FREQUENCY = 48000;        // Tần số mẫu âm thanh
			constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;        // Format âm thanh
			constexpr int32_t MONO = 1, STEREO = 2;             // Số kênh âm thanh: 1 hay 2
			constexpr int32_t BUFFER_SIZE = 256;               // Kích cỡ buffer
		}
		namespace Volume
		{
			constexpr int32_t MASTER_VOLUME = 80;      // Volume mặc định cho Audio_Mixer
			constexpr int32_t MUSIC_VOLUME = 80;       // Volume mặc định cho Music
			constexpr int32_t HITSOUND_VOLUME = 80;      // Volume mặc định cho Effects
		}
	}
}

namespace Immutable
{
	namespace Video
	{
		constexpr int32_t LOGICAL_WIDTH = 1280;
		constexpr int32_t LOGICAL_HEIGHT = 720;
	}
}