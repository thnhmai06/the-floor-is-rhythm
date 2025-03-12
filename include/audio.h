/**
 *@file audio.h
 *@brief Chứa các cấu trúc quản lý âm thanh
 *@author Mai Thành (@thnhmai06)
 */
// ♫ Camellia - Operation: Zenithfall

#ifndef AUDIO_H
#define AUDIO_H

#include <unordered_map>
#include <SDL.h>
#include <SDL_mixer.h>

//! Settings: (Low-offset Profile)
//Volume
const int DEFAULT_MASTER_VOLUME = 80; // Volume mặc định khi khởi tạo bộ Audio_Mixer
const int DEFAULT_MUSIC_VOLUME = 80; // Volume mặc định khi khởi tạo Music
const int DEFAULT_HITSOUND_VOLUME = 80; // Volume mặc định khi khởi tạo Effect
//Audio
const int MAX_CHANNELS = 8; // Giới hạn số kênh phát Effect tối đa (mỗi một channel chạy 1 effect)
const int SAMPLE_FREQUENCY = 48000; // Tần số âm thanh mẫu
const int AUDIO_FORMAT = AUDIO_F32; // Format Âm thanh
const int MONO = 1, STEREO = 2; // Phát âm thanh từ 1 hay 2 phía (L-R)
const int BUFFER_SIZE = 256; // Kích cỡ Chunksize của mỗi buffer.

/**
 * @brief Chuyển đối Thực tế -> SDL_Mixer
 * @ingroup audio volume
 * @param v Giá trị thực tế.
 * @return int: Giá trị theo SDL_Mixer.
*/
static int get_volume(const int v)
{
	if (v < 0) return -1;
	return (MIX_MAX_VOLUME * v) / 100;
}

/**
 * @brief Chuyển đối SDL_Mixer -> Thực tế
 * @ingroup audio volume
 * @param v Giá trị SDL_Mixer
 * @return int: Giá trị Thực tế
 */
static int get_real_volume(const int v)
{
	return (v * 100) / MIX_MAX_VOLUME;
}

/**
 * @class Audio_Memory audio.h
 * @ingroup audio memory
 * @brief Lớp quản lý Bộ nhớ cho các Âm thanh đã được tải.
 */
struct Audio_Memory: std::unordered_map<const char*, Mix_Music*>
{
	/**
		* @brief Giải phóng âm thanh trong bộ nhớ.
		* @ingroup audio memory
		* @param file_path Tên file âm thanh.
		*/
	void free(const char* file_path)
	{
		const auto value = this->find(file_path);
		auto audio = value->second;
		Mix_FreeMusic(audio);
		this->erase(value);
	}
	/**
	* @brief Giải phóng TẤT CẢ âm thanh trong bộ nhớ.
	* @ingroup audio memory
	*/
	void free_all()
	{
		for (const auto& item: *this) Mix_FreeMusic(item.second);
		this->clear();
	}
	/**
		 * @brief	Tải audio vào bộ nhớ.
		 * @ingroup audio memory
		 * @param	file_path: Vị trí file Music.
		 * @param	is_load_as_effect: 1 - Effect, 0 - Music.
		 * @return	Mix_Music*: Âm thanh đã được nạp, nullptr nếu ko nạp dc.
		 */
	Mix_Music* load(const char* file_path, const bool is_load_as_effect)
	{
		// Nếu âm thanh đã được tải thì ko cần nạp nữa
		auto location = this->find(file_path);
		if (location != this->end()) return location->second;

		// Nạp âm thanh
		Mix_Music* audio;
		if (!is_load_as_effect) audio = Mix_LoadMUS(file_path);
		else audio = Mix_LoadWAV(file_path);
		if (audio == NULL)
		{
			//TODO: Expection: Can't play audio
			return nullptr;
		}
		this->insert({ file_path, audio });
		return audio;
	}
};

/**
 * @class Audio_Mixer audio.h
 * @ingroup audio
 * @brief Bộ trộn Âm thanh.
 */
struct Audio_Mixer {
	int volume;

	 /**
	  * @brief Set/Lấy giá trị Volume của Audio cụ thể
	  * @ingroup audio mixer volume
	  * @param audio: Music/Effect muốn thao tác.
	  * @param value Giá trị cần set, -1 nếu cần Lấy giá trị.
	  * @return int: Giá trị trước khi set.
	  */
	static int set_audio_volume(Mix_Music* audio, const int value = -1)
	{
		if (value < 0) return get_real_volume(Mix_VolumeChunk(audio, -1));
		return get_real_volume(Mix_VolumeChunk(audio, get_volume(value)));
	}

	/**
	 * @brief Set/Lấy giá trị Master Volume
	 * @ingroup audio mixer volume
	 * @param value Giá trị cần set, -1 nếu cần lấy giá trị.
	 * @return int: Giá trị trước khi set.
	 */
	int set_volume(const int value = -1)
	{
		if (value >= 0) volume = value;
		return get_real_volume(Mix_MasterVolume(get_volume(value)));
	}

	/**
	 * @class Music
	 * @ingroup audio mixer
	 * @brief Lớp quản lý Các bài nhạc (mp3).
	 */
	struct Music
	{
		Audio_Memory memory;
		int volume;

		/**
		 * @brief Set/Lấy giá trị Music Volume
		 * @ingroup audio music volume
		 * @param value Giá trị cần set, -1 nếu cần Lấy giá trị.
		 * @return int: Giá trị trước khi set.
		*/
		int set_volume(const int value = -1)
		{
			if (value >= 0) volume = value;
			return get_real_volume(Mix_VolumeMusic(get_volume(value)));
		}

		/**
		 * @brief	Tải Âm thanh (mp3) vào bộ nhớ.
		 * @ingroup audio music
		 * @param	file_path: Vị trí file Music.
		 * @return	bool: 1 - Thành công, 0 - Thất bại.
		 */
		bool load(const char* file_path)
		{
			return memory.load(file_path, false);
		}

		/**
		 * @brief Phát Music đã load trong bộ nhớ.
		 * @ingroup audio music
		 * @param file_path: Vị trí file Music.
		 * @param while_playing_another: Có phát khi có bài khác đang phát không?
		 * @return int: 0 nếu Thành công, -1 nếu Thất bại
		 */
		int play(const char* file_path, const bool while_playing_another = true) const
		{
			if (!while_playing_another && Mix_PlayingMusic() == 1) return -1;
			const auto audio = memory.find(file_path)->second;
			return Mix_PlayMusic(audio, -1);
		}

		/**
		 * @brief Kiểm tra xem có bài nhạc nào đang phát không.
		 * @ingroup audio music
		 * @return bool: Có bài nhạc đang phát không?
		 * @note Bài đang tạm dừng cũng được tính là có bài đang phát.
		 */
		static bool has_song_playing() { return (Mix_PlayingMusic() != 0); }

		/**
		 * @brief Bài đang phát có đang được phát không.
		 * @ingroup audio music
		 * @return bool: Bài đang phát có phát không?
		 */
		static bool is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }

		/**
		 * @brief Tạm dừng Music
		 * @ingroup audio music
		 */
		static void pause() { Mix_PauseMusic(); }
		/**
		 * @brief Tiếp tục Music
		 * @ingroup audio music
		 */
		static void resume() { Mix_ResumeMusic(); }
		/**
		 * @brief Dừng Music
		 * @ingroup audio music
		 * @return int: 0 - Thành công, -1 - Lỗi (vd ko có nhạc đang phát) 
		 */
		static int stop() { return Mix_HaltMusic(); }

		Music()
		{
			set_volume(DEFAULT_MUSIC_VOLUME);
		}
	} music;

	/**
	 * @class Effect
	 * @ingroup audio mixer
	 * @brief Lớp quản lý Các effect (wav).
	 */
	struct Effect
	{
		Audio_Memory memory;
		int volume;

		/**
		 * @brief Set/Lấy giá trị Effect Volume
		 * @ingroup audio effect volume
		 * @param value Giá trị cần set, -1 nếu cần lấy giá trị.
		 * @return int: Giá trị trung bình volume các channels.
		 */
		int set_volume(const int value = -1)
		{
			if (value >= 0) volume = value;
			return get_real_volume(Mix_Volume(-1, get_volume(value)));
		}

		/**
		 * @brief	Tải Effect vào bộ nhớ.
		 * @ingroup audio effect
		 * @param	file_path: Vi tri file Effect (wav).
		 * @return	Mix_Music*: Con trỏ tới Effect đã đươc load. nullptr nếu bị lỗi.
		 */
		bool load(const char* file_path)
		{
			return memory.load(file_path, true);
		}

		/**
		 * @brief Phát Effect được lưu trong bộ nhớ.
		 * @param file_path: Vi tri file Effect (wav).
		 * @return int: Channel mà effect được phát, -1 nếu bị lỗi.
		 */
		int play(const char* file_path) const
		{
			const auto audio = memory.find(file_path)->second;
			return Mix_PlayChannel(-1, audio, 0);
		}

		Effect()
		{
			set_volume(DEFAULT_HITSOUND_VOLUME);
		}
	} effect;

	Audio_Mixer()
	{
		Mix_Init(MIX_INIT_MP3);
		SDL_Init(SDL_INIT_AUDIO);
		Mix_AllocateChannels(MAX_CHANNELS);
		if (Mix_OpenAudio(SAMPLE_FREQUENCY, AUDIO_FORMAT, STEREO, BUFFER_SIZE) < 0) {
			// TODO: Expectation: Cannot init Audio_Mixer
		}

		// After init
		set_volume(DEFAULT_MASTER_VOLUME);
	}
	
	void quit()
	{
		effect.memory.free_all();
		music.memory.free_all();
		Mix_Quit();
	}
};
#endif // AUDIO_H