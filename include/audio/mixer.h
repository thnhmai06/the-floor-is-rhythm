#pragma once
#include <cstdint>
#include <SDL3/SDL_audio.h>
#include "audio/buses.h"

/**
 * @class Mixer
 * @brief Bộ trộn Âm thanh.
 */
struct Mixer {
	int32_t volume;
	SDL_AudioSpec spec;
	AudioBuses::Music music;
	AudioBuses::Effects effect;

	/**
	 * @brief Set/Lấy giá trị Master Volume
	 */
	inline int32_t set_master_volume(const int32_t value = -1);

	Mixer();

	/**
	 * @brief Giải phóng hết audio và thoát Mixer.
	 */
	void quit();
};