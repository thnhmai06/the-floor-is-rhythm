#pragma once
#include <easing.h>
#include "structures/action/event/callback.h"
#include "structures/render/layers/objects/object.h"

namespace Structures::Action::Event::Render
{
	// trying to remake https://osu.ppy.sh/wiki/en/Storyboard/Scripting/Commands

	//! BASE
	struct RenderCallback : Callback
	{
		Structures::Render::Objects::Object* target_object = nullptr;
		easing_functions easing;

	protected:
		[[nodiscard]] float get_current_value_in_percent(const int64_t& current_time) const;

	public:
		bool is_active(const int64_t& current_time) override;
	};

	//! Fade
	struct FadeCallback : RenderCallback
	{
		uint8_t from, to;

		void execute(const int64_t& current_time) override;

		FadeCallback() = default;
		FadeCallback(const easing_functions& easing, const float& from_percent, const float& to_percent);
	};

	//! Move
	struct MoveCallback : RenderCallback
	{
		SDL_FPoint from, to;

		void execute(const int64_t& current_time) override;
	};
	struct MoveXCallback : RenderCallback
	{
		float from_x, to_x;

		void execute(const int64_t& current_time) override;
	};
	struct MoveYCallback : RenderCallback
	{
		float from_y, to_y;

		void execute(const int64_t& current_time) override;
	};

	//! Scale
	struct ScaleCallback : RenderCallback
	{
		SDL_FPoint from, to;

		void execute(const int64_t& current_time) override;

		ScaleCallback() = default;
		ScaleCallback(const easing_functions& easing, const float& from, const float& to);
	};

	//! Rotate
	struct RotateCallback : RenderCallback
	{
		float from, to;

		void execute(const int64_t& current_time) override;
	};
}