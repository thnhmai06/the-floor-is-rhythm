#pragma once
#include "memory.h"

namespace AudioBuses
{
	/**
	 * @class Music
	 * @brief Lớp quản lý các bài nhạc (mp3).
	 */
	struct Music
	{
		AudioMemory::Music memory;
		int32_t volume;

		/**
		 * @brief Set/Lấy giá trị Music Volume
		 */
		int32_t set_volume(const int32_t value = -1);

		/**
		 * @brief Tải âm thanh (mp3) vào bộ nhớ.
		 */
		bool load(const char* file_path);

		/**
		 * @brief Phát Music đã load trong bộ nhớ.
		 */
		int32_t play(const char* file_path, const bool while_playing_another = true) const;

		/**
		 * @brief Kiểm tra xem có bài nhạc nào đang phát không.
		 */
		static bool has_song_playing();

		/**
		 * @brief Kiểm tra xem bài đang phát có đang chạy không.
		 */
		static bool is_playing();

		/**
		 * @brief Tạm dừng Music.
		 */
		static void pause();

		/**
		 * @brief Tiếp tục phát Music.
		 */
		static void resume();

		/**
		 * @brief Dừng Music.
		 */
		static void stop();

		Music();
	};
	/**
	 * @class Effects
	 * @brief Lớp quản lý các hiệu ứng âm thanh (wav).
	 */
	struct Effects
	{
		AudioMemory::Effects memory;
		int32_t volume;
		/**
		 * @brief Set/Lấy giá trị tổng thể của Effect Volume.
		 */
		int32_t set_volume(const int32_t value = -1);
		/**
		 * @brief Set/Lấy giá trị volume của Effect cụ thể.
		 */
		int32_t set_effect_volume(const char* file_name, const int32_t value = -1) const;
		/**
		 * @brief Tải hiệu ứng âm thanh vào bộ nhớ.
		 */
		bool load(const char* file_path);
		/**
		 * @brief Phát hiệu ứng âm thanh đã load trong bộ nhớ.
		 */
		int32_t play(const char* file_path) const;

		Effects();
	};
}
