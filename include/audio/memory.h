#pragma once
#include <string>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>

namespace AudioMemory
{
	/**
	 * @class Music
	 * @brief Lớp quản lý bộ nhớ cho các Music đã được tải.
	 */
	struct Music : std::unordered_map<std::string, Mix_Music*>
	{
		/**
		 * @brief Giải phóng Music theo file_path.
		 */
		void free(const std::string& name);

		/**
		 * @brief Giải phóng tất cả Music trong bộ nhớ.
		 */
		void free_all();

		/**
		 * @brief Tải audio vào bộ nhớ.
		 */
		Mix_Music* load(const char* file_path, const std::string& name);
	};

    /**
     * @class Effects
     * @brief Lớp quản lý bộ nhớ cho các Effect đã được tải.
     */
    struct Effects : std::unordered_map<std::string, Mix_Chunk*> {
        /**
         * @brief Giải phóng Effect theo file_path.
         */
        void free(const std::string& name);

        /**
         * @brief Giải phóng tất cả Effect trong bộ nhớ.
         */
        void free_all();

        /**
         * @brief Tải Effect vào bộ nhớ.
         */
		Mix_Chunk* load(const char* file_path, const std::string& name);
    };
}