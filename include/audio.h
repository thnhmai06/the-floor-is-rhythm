/**
 *@file audio.h
 *@brief Chứa các cấu trúc quản lý âm thanh
 *@author Mai Thành (@thnhmai06)
 */
 // ♫ Camellia - Operation: Zenithfall
//TODO: Chưa kiểm tra Mixer, cần thử Mixer xem có bị lỗi chỗ nào không
#pragma once

#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

//! Settings: (Low-offset Profile)
// Volume
constexpr int32_t DEFAULT_MASTER_VOLUME = 80;      // Volume mặc định cho Audio_Mixer
constexpr int32_t DEFAULT_MUSIC_VOLUME = 80;       // Volume mặc định cho Music
constexpr int32_t DEFAULT_HITSOUND_VOLUME = 80;      // Volume mặc định cho Effects
// Audio
constexpr int32_t MAX_CHANNELS = 8;                // Số kênh phát hiệu ứng tối đa
constexpr int32_t SAMPLE_FREQUENCY = 48000;        // Tần số mẫu âm thanh
constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;        // Format âm thanh
constexpr int32_t MONO = 1, STEREO = 2;             // Số kênh âm thanh: 1 hay 2
constexpr int32_t BUFFER_SIZE = 256;               // Kích cỡ buffer

/**
 * @brief Chuyển đối Thực tế -> SDL_Mixer
 */
static int32_t get_volume(const int32_t v)
{
    if (v < 0) return -1;
    return (MIX_MAX_VOLUME * v) / 100;
}

/**
 * @brief Chuyển đối SDL_Mixer -> Thực tế
 */
static int32_t get_real_volume(const int32_t v)
{
    return (v * 100) / MIX_MAX_VOLUME;
}

/**
 * @class Audio_Mixer
 * @brief Bộ trộn Âm thanh.
 */
struct Audio_Mixer {
    int32_t volume;
	SDL_AudioSpec spec;

    /**
     * @brief Set/Lấy giá trị Master Volume
     */
    int32_t set_volume(const int32_t value = -1)
    {
        if (value >= 0) volume = value;
        return get_real_volume(Mix_MasterVolume(get_volume(value)));
    }

    /**
     * @class Music
     * @brief Lớp quản lý các bài nhạc (mp3).
     */
    struct Music {
        /**
         * @class Memory
         * @brief Lớp quản lý bộ nhớ cho các Music đã được tải.
         */
        struct Memory : std::unordered_map<const char*, Mix_Music*> {
            /**
             * @brief Giải phóng Music theo file_path.
             */
            void free(const char* file_path)
            {
                auto it = this->find(file_path);
                if (it != this->end()) {
                    Mix_FreeMusic(it->second);
                    this->erase(it);
                }
            }

            /**
             * @brief Giải phóng tất cả Music trong bộ nhớ.
             */
            void free_all()
            {
                for (const auto& item : *this)
                    Mix_FreeMusic(item.second);
                this->clear();
            }

            /**
             * @brief Tải audio vào bộ nhớ.
             */
            Mix_Music* load(const char* file_path)
            {
                auto it = this->find(file_path);
                if (it != this->end())
                    return it->second;

                Mix_Music* audio = Mix_LoadMUS(file_path);
                if (!audio) {
                    // TODO: Xử lý lỗi không tải được audio
                    return nullptr;
                }
                this->insert({ file_path, audio });
                return audio;
            }
        } memory;

        int32_t volume;

        /**
         * @brief Set/Lấy giá trị Music Volume
         */
        int32_t set_volume(const int32_t value = -1)
        {
            if (value >= 0) volume = value;
            return get_real_volume(Mix_VolumeMusic(get_volume(value)));
        }

        /**
         * @brief Tải âm thanh (mp3) vào bộ nhớ.
         */
        bool load(const char* file_path)
        {
            return (memory.load(file_path) != nullptr);
        }

        /**
         * @brief Phát Music đã load trong bộ nhớ.
         */
        int32_t play(const char* file_path, const bool while_playing_another = true) const
        {
            if (!while_playing_another && Mix_PlayingMusic() == 1)
                return -1;

            auto it = memory.find(file_path);
            if (it == memory.end())
                return -1;

            Mix_Music* audio = it->second;
            return Mix_PlayMusic(audio, -1);
        }

        /**
         * @brief Kiểm tra xem có bài nhạc nào đang phát không.
         */
        static bool has_song_playing() { return (Mix_PlayingMusic() != 0); }

        /**
         * @brief Kiểm tra xem bài đang phát có đang chạy không.
         */
        static bool is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }

        /**
         * @brief Tạm dừng Music.
         */
        static void pause() { Mix_PauseMusic(); }

        /**
         * @brief Tiếp tục phát Music.
         */
        static void resume() { Mix_ResumeMusic(); }

        /**
         * @brief Dừng Music.
         */
        static void stop() { return Mix_HaltMusic(); }

        Music() { set_volume(DEFAULT_MUSIC_VOLUME); }
    } music;

    /**
     * @class Effects
     * @brief Lớp quản lý các hiệu ứng âm thanh (wav).
     */
    struct Effects {
        /**
         * @class Memory
         * @brief Lớp quản lý bộ nhớ cho các Effect đã được tải.
         */
        struct Memory : std::unordered_map<const char*, Mix_Chunk*> {
            /**
             * @brief Giải phóng Effect theo file_path.
             */
            void free(const char* file_path)
            {
                auto it = this->find(file_path);
                if (it != this->end()) {
                    Mix_FreeChunk(it->second);
                    this->erase(it);
                }
            }

            /**
             * @brief Giải phóng tất cả Effect trong bộ nhớ.
             */
            void free_all()
            {
                for (const auto& [key, value] : *this)
                    Mix_FreeChunk(value);
                this->clear();
            }

            /**
             * @brief Tải Effect vào bộ nhớ.
             */
            Mix_Chunk* load(const char* file_path)
            {
                auto it = this->find(file_path);
                if (it != this->end())
                    return it->second;

                Mix_Chunk* audio = Mix_LoadWAV(file_path);
                if (!audio) {
                    // TODO: Xử lý lỗi không tải được audio
                    return nullptr;
                }
                this->insert({ file_path, audio });
                return audio;
            }
        } memory;

        int32_t volume;

        /**
         * @brief Set/Lấy giá trị tổng thể của Effect Volume.
         */
        int32_t set_volume(const int32_t value = -1)
        {
            if (value >= 0) volume = value;
            return get_real_volume(Mix_Volume(-1, get_volume(value)));
        }

        /**
         * @brief Set/Lấy giá trị volume của Effect cụ thể.
         */
        int32_t set_effect_volume(const char* file_name, const int32_t value = -1) const
        {
            auto it = memory.find(file_name);
            if (it == memory.end()) return -1;
            Mix_Chunk* audio = it->second;
            if (value < 0)
                return get_real_volume(Mix_VolumeChunk(audio, -1));
            return get_real_volume(Mix_VolumeChunk(audio, get_volume(value)));
        }

        /**
         * @brief Tải Effect vào bộ nhớ.
         */
        bool load(const char* file_path)
        {
            return (memory.load(file_path) != nullptr);
        }

        /**
         * @brief Phát Effect được lưu trong bộ nhớ.
         */
        int32_t play(const char* file_path) const
        {
            auto it = memory.find(file_path);
            if (it == memory.end())
                return -1;
            Mix_Chunk* audio = it->second;
            return Mix_PlayChannel(-1, audio, 0);
        }

        Effects() { set_volume(DEFAULT_HITSOUND_VOLUME); }
    } effect;

    Audio_Mixer()
    {
        // Khởi tạo hỗ trợ MP3 (với SDL3_mixer, hãy kiểm tra tài liệu nếu có thay đổi)
        SDL_Init(SDL_INIT_AUDIO);
        Mix_Init(MIX_INIT_MP3);
        Mix_AllocateChannels(MAX_CHANNELS);

		spec.channels = STEREO;
		spec.format = AUDIO_FORMAT;
		spec.freq = SAMPLE_FREQUENCY;

        if (!Mix_OpenAudio(0, &spec)) {
            // TODO: Xử lý lỗi không khởi tạo được Audio_Mixer
        }

        // Sau khi khởi tạo, đặt master volume
        set_volume(DEFAULT_MASTER_VOLUME);
    }

    void quit()
    {
        effect.memory.free_all();
        music.memory.free_all();
        Mix_Quit();
    }
};
