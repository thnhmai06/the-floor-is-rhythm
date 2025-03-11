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
const int DEFAULT_VOLUME = 80; // Volume mặc định khi khởi tạo bộ Mixer
const int MAX_CHANNELS = 8; // Giới hạn số kênh phát Hitsound tối đa (1 kênh chạy 1 hitsound)
const int SAMPLE_FREQUENCY = 48000; // Tần số âm thanh mẫu
const int AUDIO_FORMAT = AUDIO_F32; // Format Âm thanh
const int MONO = 1, STEREO = 2; // Phát âm thanh từ 1 hay 2 phía (L-R)
const int BUFFER_SIZE = 256; // Kích cỡ Chunksize của mỗi buffer.

/**
 * @class Audio_Memory audio.h
 * @ingroup audio
 * @brief Lớp quản lý Bộ nhớ cho các Âm thanh đã được tải.
 */
struct Audio_Memory: std::unordered_map<const char*, Mix_Music*>
{
	/**
		* @brief Giải phóng âm thanh trong bộ nhớ.
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
	*/
	void free_all()
	{
		for (const auto& item: *this) Mix_FreeMusic(item.second);
		this->clear();
	}
};

/**
 * @class Mixer audio.h
 * @ingroup audio
 * @brief Bộ trộn Âm thanh.
 */
struct Mixer {
	/**
	 * @class Volume
	 * @ingroup audio mixer
	 * @brief Lớp quản lý Âm lượng.
	 */
	struct Volume
	{
		//! value ở đây là value thực tế trong SDL_Mixer.
		// Sử dụng get_real_volume() để lấy giá trị thực.
		int value;
		void set_volume(const int v)
		{
			value = (MIX_MAX_VOLUME * v) / 100;
			Mix_Volume(-1, value);
		}
		void mute() { set_volume(0); }
		int get_real_volume() const
		{
			return (value * 100) / MIX_MAX_VOLUME;
		}

		Volume(const int v = DEFAULT_VOLUME) { set_volume(v); }
		Volume& operator= (const Volume& other)
		{
			if (this != &other)
			{
				this->set_volume(other.get_real_volume());
			}
			return *this;
		}
		Volume& operator= (const int v)
		{
			this->set_volume(v);
			return *this;
		}
	} volume;

	/**
	 * @class Music
	 * @ingroup audio mixer
	 * @brief Lớp quản lý Các bài nhạc (mp3).
	 */
	struct Music
	{
		Audio_Memory memory;

		/**
		 * @brief	Tải Âm thanh (mp3) vào bộ nhớ.
		 * @param	file_path: Vị trí file Music.
		 * @return	Mix_Music*: Con trỏ tới Music đã đươc load. nullptr nếu bị lỗi.
		 */
		Mix_Music* load(const char* file_path)
		{
			// Neu file da duoc nap thi ko can nap nua
			auto location = memory.find(file_path);
			if (location != memory.end()) return location->second;

			// Nap file
			Mix_Music* audio = nullptr;
			audio = Mix_LoadMUS(file_path);
			if (audio == nullptr)
			{
				//TODO: Expection: Can't play audio
				return nullptr;
			}
			return memory[file_path] = audio;
		}

		/**
		 * @brief Phát Music đã load trong bộ nhớ.
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
	} music;

	/**
	 * @class Hitsound
	 * @ingroup audio mixer
	 * @brief Lớp quản lý Các hitsound (wav).
	 */
	struct Hitsound
	{
		Audio_Memory memory;

		/**
		 * @brief	Tải Hitsound vào bộ nhớ.
		 * @param	file_path: Vi tri file Hitsound (wav).
		 * @return	Mix_Music*: Con trỏ tới Hitsound đã đươc load. nullptr nếu bị lỗi.
		 */
		Mix_Music* load(const char* file_path)
		{
			// Nếu file đã được nạp r thì ko cần nạp nữa
			auto location = memory.find(file_path);
			if (location != memory.end()) return location->second;

			// Nạp file
			Mix_Music* audio = nullptr;
			audio = Mix_LoadWAV(file_path);
			if (audio == nullptr)
			{
				//TODO: Expection: Can't play audio
				return nullptr;
			}
			return memory[file_path] = audio;
		}

		/**
		 * @brief Phát Hitsound được lưu trong bộ nhớ.
		 * @param file_path: Vi tri file Hitsound (wav).
		 * @return int: Channel mà hitsound được phát, -1 nếu bị lỗi.
		 */
		int play(const char* file_path) const
		{
			auto audio = memory.find(file_path)->second;
			return Mix_PlayChannel(-1, audio, 0);
		}
	} hitsound;

	Mixer()
	{
		Mix_Init(MIX_INIT_MP3);
		SDL_Init(SDL_INIT_AUDIO);
		Mix_AllocateChannels(MAX_CHANNELS);
		if (Mix_OpenAudio(SAMPLE_FREQUENCY, AUDIO_FORMAT, STEREO, BUFFER_SIZE) < 0) {
			// TODO: Excecption: Cannot init Mixer
		}
	}

	void quit()
	{
		music.memory.free_all();
		hitsound.memory.free_all();
		Mix_Quit();
	}
};
#endif // AUDIO_H