#pragma once
#include "structures/render/object.h"
#include "structures/events/time/time.h"

namespace Structures::Render::Object
{
	struct AnimationObject : Object
	{
	protected:
		std::optional<int64_t> last_frame_time;
		long long current_frame = 0;

		void update(const int64_t& current_time);
	public:
		Events::Time::Timer* timer;
		std::vector<Memory::Item> frames;
		int64_t frame_delay;
		Types::Render::LoopType loop_type;

		void on_before_render() override;

		explicit AnimationObject(
			Events::Time::Timer* timer,
			std::vector<Memory::Item> frames, const int64_t& frame_delay,
			const Types::Render::LoopType& loop_type = Types::Render::LoopType::LoopForever,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit AnimationObject(
			Events::Time::Timer* timer,
			std::vector<Memory::Item> frames, const int64_t& frame_delay,
			const Config::OriginPoint& custom_origin,
			const Types::Render::LoopType& loop_type = Types::Render::LoopType::LoopForever,
			const SDL_FPoint& render_pos = { 0, 0 });
	};
}