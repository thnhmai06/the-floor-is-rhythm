#pragma once
#include <easing.h>
#include <memory>
#include <SDL3/SDL_events.h>

namespace Engine::Events
{
	namespace Timing
	{
		using Tick = Uint64;
		using Time = int64_t;

		namespace Easing
		{
			enum class Easing : std::uint8_t
			{
				Linear = 0, EasingOut = 1, EasingIn = 2,
				QuadIn = 3, QuadOut = 4, QuadInOut = 5,
				CubicIn = 6, CubicOut = 7, CubicInOut = 8,
				QuartIn = 9, QuartOut = 10, QuartInOut = 11,
				QuintIn = 12, QuintOut = 13, QuintInOut = 14,
				SineIn = 15, SineOut = 16, SineInOut = 17,
				ExpoIn = 18, ExpoOut = 19, ExpoInOut = 20,
				CircIn = 21, CircOut = 22, CircInOut = 23,
				ElasticIn = 24, ElasticOut = 25,
				ElasticHalfOut = 26, ElasticQuarterOut = 27,
				ElasticInOut = 28,
				BackIn = 29, BackOut = 30, BackInOut = 31,
				BounceIn = 32, BounceOut = 33, BounceInOut = 34
			};
			using EasingFunction = easingFunction;
		}
	}
	namespace Event
	{
		namespace Internal
		{
			using EventType = size_t;
			struct Event
			{
				Timing::Time time;
				EventType type;

				Event(const Timing::Time& time, const EventType& type);
				virtual ~Event() = default;
			};
			using EventCon = std::shared_ptr<Event>;
			using EventView = std::weak_ptr<const Event>;
		}
		namespace External
		{
			using EventType = SDL_EventType;
			using Event = SDL_Event;
			using EventView = const Event&;
		}
	}
	namespace Action
	{
		namespace Render
		{
			enum class Parameter : char
			{
				Horizontal = 'H',
				Vertical = 'V',
				AdditiveColour = 'A'
			};
		}
	}
}
