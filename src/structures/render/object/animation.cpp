#include "structures/render/object/animation.h" // Header
#include "utilities.hpp"

namespace Structures::Render::Object
{
	void AnimationObject::update(const int64_t& current_time)
	{
		if (last_frame_time.has_value())
		{
			const auto time_elapsed = current_time - last_frame_time.value();
			const auto frame_elapsed = time_elapsed / frame_delay;

			const auto& frame_count = static_cast<long long>(frames.size());
			if (loop_type == Types::Render::LoopType::LoopOnce)
				current_frame = std::clamp(current_frame + frame_elapsed, 0LL, frame_count - 1);
			else
				current_frame = Utilities::Math::circle_mod(current_frame + frame_elapsed, frame_count);
		}
		last_frame_time = current_time;
		src = frames.at(current_frame);
	}
	void AnimationObject::on_before_render()
	{
		update(timer->get_time());
	}
	AnimationObject::AnimationObject(
		Events::Time::Timer* timer,
		std::vector<Memory::Item> frames, const int64_t& frame_delay,
		const Types::Render::LoopType& loop_type,
		const Types::Render::OriginType& origin_type,
		const SDL_FPoint& render_pos)
		: Object(frames.front(), origin_type, render_pos),
		timer(timer), frames(std::move(frames)),
		frame_delay(frame_delay), loop_type(loop_type)
	{
	}
	AnimationObject::AnimationObject(
		Events::Time::Timer* timer,
		std::vector<Memory::Item> frames, const int64_t& frame_delay,
		const Config::OriginPoint& custom_origin,
		const Types::Render::LoopType& loop_type,
		const SDL_FPoint& render_pos)
		: Object(frames.front(), custom_origin, render_pos),
		timer(timer), frames(std::move(frames)),
		frame_delay(frame_delay), loop_type(loop_type)
	{
	}

}
