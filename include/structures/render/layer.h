#pragma once
#include <SDL3/SDL_render.h>
#include "config.h"
#include "structures/render/playground/hitobject.h"
#include "structures/game/timing.h"
#include "utilities.h"

namespace Structures::Render::Layers
{
	using namespace Structures::Render::Textures;

	/**
	 * Trong SDL3, thật buồn là ta ko có một cấu trúc nào để biểu diễn một Layer :(((\n
	 * Vì vậy, ý tưởng của mình là tạo một cấu trúc quản lý các render_buffer giống như Layer,
	 * và render trực tiếp lên màn hình.
	 * Cấu trúc này sẽ có config chung để có thể đồng loạt thay đổi các render_buffer. \n
	 *
	 * Tại sao không dùng SDL_Texture* đại diện cho Layer? \n
	 * Vì nó sẽ render texture ra Layer đó, rồi mới render Layer ra
	 * màn hình chính. Như vậy giống như việc ta render 2 lần vậy.
	 * (Vấn đề về Hiệu suất và Overhead)
	 */
	struct Layer
	{
		using RenderBuffer = std::list<RenderObjects::RenderObjects>;
		using RenderRange = std::pair<int32_t, int32_t>;

		const TextureMemory* memory = nullptr;
		RenderBuffer render_buffer;
		std::optional<RenderRange> render_range;
		struct LayerCamera : private RenderConfig
		{
			void move_into_camera(RenderObjects::RenderObject& object) const;
			void move_out_camera(RenderObjects::RenderObject& object) const;

			[[nodiscard]] uint8_t get_alpha() const;
			void set_alpha(const uint8_t& value);
			[[nodiscard]] SDL_FPoint get_camera_pos() const;
			[[nodiscard]] SDL_FPoint get_camera_size(bool after_scale = true) const;
			void move_x(const float& dx);
			void move_y(const float& dy);

			LayerCamera() : RenderConfig()
			{
				origin_pos = {
					Utilities::Math::centre(0, GameConfig::Video::LOGICAL_WIDTH),
					Utilities::Math::centre(0, GameConfig::Video::LOGICAL_HEIGHT)
				};
				render_pos = { .x = GameConfig::Video::Camera::DEFAULT_POS_X, .y = GameConfig::Video::Camera::DEFAULT_POS_Y };
			}
		} camera;
		bool visible = true;

		virtual ~Layer() = default;
		explicit Layer(const TextureMemory* memory);
		virtual void render();
		void reset(bool to_initial_state = false);
	};
	struct PlaygoundLayer : Layer
	{
		//TODO: Làm tương thích với beatmap skin

		void run_beatmap(
			const GameObjects::HitObjects::HitObjects& hit_objects,
			const GameObjects::Metadata::CalculatedDifficulty& difficulty,
			const GameObjects::Timing::TimingPoints& timing_points);

		PlaygoundLayer(const TextureMemory* memory);
		PlaygoundLayer(
			const TextureMemory* memory,
			const GameObjects::HitObjects::HitObjects& hit_objects,
			const GameObjects::Metadata::CalculatedDifficulty& difficulty,
			const GameObjects::Timing::TimingPoints& timing_points);
	};
}