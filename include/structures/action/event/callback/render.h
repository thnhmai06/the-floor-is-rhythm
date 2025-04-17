#pragma once
#include <easing.h>
#include "structures/action/event/callback.h"
#include "structures/render/layers/objects/object.h"

namespace Structures::Action::Event::Render
{
	// BASE
	struct RenderCallback : Callback
	{
		Structures::Render::Objects::Object* target_object = nullptr;
		easing_functions easing;

		bool is_active(const int64_t& current_time) override;
	};

	// command
	struct FadeCallback : RenderCallback
	{
		uint8_t start_alpha, end_alpha;

		void execute(const int64_t& current_time) override;
	};
	struct MoveCommand : RenderCallback
	{

	};
}