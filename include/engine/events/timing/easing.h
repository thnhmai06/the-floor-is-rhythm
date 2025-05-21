#pragma once
#include <cmath>
#include <numbers>
#include "engine/events/type.h"

namespace Engine::Events::Timing
{
	namespace Easing
	{
		static double Linear(const double x) { return x; }
		static double ElasticHalfOut(const double x)
		{
			const double C_h = std::pow(2, -10.0) * std::sin((0.5 - 0.075) * (2 * std::numbers::pi_v<double> / 0.3));
			return std::pow(2, -10.0 * x) * std::sin((0.5 * x - 0.075) * (2 * std::numbers::pi_v<double> / 0.3)) + 1.0 - C_h * x;
		}
		static double ElasticQuarterOut(const double x)
		{
			const double C_q = std::pow(2, -10.0) * std::sin((0.25 - 0.075) * (2 * std::numbers::pi_v<double> / 0.3));
			return std::pow(2, -10.0 * x) * std::sin((0.25 * x - 0.075) * (2 * std::numbers::pi_v<double> / 0.3)) + 1.0 - C_q * x;
		}

		inline EasingFunction get_easing_function(const Easing& type)
		{
			switch (type)
			{
			case Easing::Linear:          return Linear;
			case Easing::EasingOut:       // giống QuadOut
			case Easing::QuadOut:         return getEasingFunction(EaseOutQuad);
			case Easing::EasingIn:        // giống QuadIn
			case Easing::QuadIn:          return getEasingFunction(EaseInQuad);
			case Easing::QuadInOut:       return getEasingFunction(EaseInOutQuad);
			case Easing::CubicIn:         return getEasingFunction(EaseInCubic);
			case Easing::CubicOut:        return getEasingFunction(EaseOutCubic);
			case Easing::CubicInOut:      return getEasingFunction(EaseInOutCubic);
			case Easing::QuartIn:         return getEasingFunction(EaseInQuart);
			case Easing::QuartOut:        return getEasingFunction(EaseOutQuart);
			case Easing::QuartInOut:      return getEasingFunction(EaseInOutQuart);
			case Easing::QuintIn:         return getEasingFunction(EaseInQuint);
			case Easing::QuintOut:        return getEasingFunction(EaseOutQuint);
			case Easing::QuintInOut:      return getEasingFunction(EaseInOutQuint);
			case Easing::SineIn:          return getEasingFunction(EaseInSine);
			case Easing::SineOut:         return getEasingFunction(EaseOutSine);
			case Easing::SineInOut:       return getEasingFunction(EaseInOutSine);
			case Easing::ExpoIn:          return getEasingFunction(EaseInExpo);
			case Easing::ExpoOut:         return getEasingFunction(EaseOutExpo);
			case Easing::ExpoInOut:       return getEasingFunction(EaseInOutExpo);
			case Easing::CircIn:          return getEasingFunction(EaseInCirc);
			case Easing::CircOut:         return getEasingFunction(EaseOutCirc);
			case Easing::CircInOut:       return getEasingFunction(EaseInOutCirc);
			case Easing::ElasticIn:       return getEasingFunction(EaseInElastic);
			case Easing::ElasticOut:      return getEasingFunction(EaseOutElastic);
			case Easing::ElasticHalfOut:  return ElasticHalfOut;
			case Easing::ElasticQuarterOut: return ElasticQuarterOut;
			case Easing::ElasticInOut:    return getEasingFunction(EaseInOutElastic);
			case Easing::BackIn:          return getEasingFunction(EaseInBack);
			case Easing::BackOut:         return getEasingFunction(EaseOutBack);
			case Easing::BackInOut:       return getEasingFunction(EaseInOutBack);
			case Easing::BounceIn:        return getEasingFunction(EaseInBounce);
			case Easing::BounceOut:       return getEasingFunction(EaseOutBounce);
			case Easing::BounceInOut:     return getEasingFunction(EaseInOutBounce);
			}

			return Linear;
		}
	}
}