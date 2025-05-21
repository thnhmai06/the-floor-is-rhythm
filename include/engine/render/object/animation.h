#pragma once
#include "engine/render/object.h"
#include "engine/events/timing/time.h"

namespace Engine::Render::Object
{
	using Events::Timing::Timer;

	struct AnimationObject : Object
	{
	protected:
		std::optional<int64_t> last_frame_time;
		long long current_frame = 0;

		void update();
		void on_before_render() override;
	public:
		std::weak_ptr<const Timer> timer;
		std::vector<Memory::Item> frames;
		int64_t frame_delay;
		LoopType loop_type;

		explicit AnimationObject(
			std::weak_ptr<const Timer> timer,
			std::vector<Memory::Item> frames, const int64_t& frame_delay,
			const LoopType& loop_type = LoopType::LoopForever,
			const OriginType& origin_type = OriginType::Centre,
			const SDL_FPoint& render_pos = { .x = 0, .y = 0 });
		explicit AnimationObject(
			std::weak_ptr<const Timer> timer,
			std::vector<Memory::Item> frames, const int64_t& frame_delay,
			const Config::OriginPoint& custom_origin,
			const LoopType& loop_type = LoopType::LoopForever,
			const SDL_FPoint& render_pos = {.x = 0, .y = 0 });
	};
}