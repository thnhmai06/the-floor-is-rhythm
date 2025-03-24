#include "audio/mixer.h" // Header
#include "utilities.h"
#include "config.h"

using namespace Utilities::Audio;
using namespace Immutable::Audio;
using Config::Volume::master;

int32_t Mixer::set_master_volume(const int32_t value)
{
	if (value >= 0) volume = value;
	return get_real_volume(Mix_MasterVolume(get_volume(value)));
}
Mixer::Mixer()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_AllocateChannels(MAX_CHANNELS);

	spec.channels = STEREO;
	spec.format = AUDIO_FORMAT;
	spec.freq = SAMPLE_FREQUENCY;

	if (!Mix_OpenAudio(0, &spec)) {
		// TODO: Xử lý lỗi không khởi tạo được Mixer
	}

	// Sau khi khởi tạo, đặt master volume
	set_master_volume(master);
}
void Mixer::quit()
{
	effect.memory.free_all();
	music.memory.free_all();
	Mix_Quit();
}