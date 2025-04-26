#pragma once
#include <numbers>
#include <osu!parser/Parser/Structures/Beatmap/Objects/Event.hpp>
#include <easing.h>

namespace Structures::Events::Time::Easing
{
	using EasingFunctionType = OsuParser::Beatmap::Objects::Event::Type::Commands::Args::Easing::Easing;
	using EasingFunction = easingFunction;

	namespace Functions
	{
		inline double Linear(double x) { return x; }
		inline double ElasticHalfOut(double x)
		{
			const double C_h = std::pow(2, -10.0) * std::sin((0.5 - 0.075) * (2 * std::numbers::pi_v<double> / 0.3));
			return std::pow(2, -10.0 * x) * std::sin((0.5 * x - 0.075) * (2 * std::numbers::pi_v<double> / 0.3)) + 1.0 - C_h * x;
        }
        inline double ElasticQuarterOut(double x)
        {
            const double C_q = std::pow(2, -10.0) * std::sin((0.25 - 0.075) * (2 * std::numbers::pi_v<double> / 0.3));
            return std::pow(2, -10.0 * x) * std::sin((0.25 * x - 0.075) * (2 * std::numbers::pi_v<double> / 0.3)) + 1.0 - C_q * x;
		}
	}

	inline EasingFunction get_easing_function(const EasingFunctionType& type)
	{
        switch (type)
        {
        case EasingFunctionType::Linear:          return Functions::Linear;
        case EasingFunctionType::EasingOut:       // giống QuadOut
        case EasingFunctionType::QuadOut:         return getEasingFunction(EaseOutQuad);
		case EasingFunctionType::EasingIn:        // giống QuadIn
        case EasingFunctionType::QuadIn:          return getEasingFunction(EaseInQuad);
        case EasingFunctionType::QuadInOut:       return getEasingFunction(EaseInOutQuad);
        case EasingFunctionType::CubicIn:         return getEasingFunction(EaseInCubic);
        case EasingFunctionType::CubicOut:        return getEasingFunction(EaseOutCubic);
        case EasingFunctionType::CubicInOut:      return getEasingFunction(EaseInOutCubic);
        case EasingFunctionType::QuartIn:         return getEasingFunction(EaseInQuart);
        case EasingFunctionType::QuartOut:        return getEasingFunction(EaseOutQuart);
        case EasingFunctionType::QuartInOut:      return getEasingFunction(EaseInOutQuart);
        case EasingFunctionType::QuintIn:         return getEasingFunction(EaseInQuint);
        case EasingFunctionType::QuintOut:        return getEasingFunction(EaseOutQuint);
        case EasingFunctionType::QuintInOut:      return getEasingFunction(EaseInOutQuint);
        case EasingFunctionType::SineIn:          return getEasingFunction(EaseInSine);
        case EasingFunctionType::SineOut:         return getEasingFunction(EaseOutSine);
        case EasingFunctionType::SineInOut:       return getEasingFunction(EaseInOutSine);
        case EasingFunctionType::ExpoIn:          return getEasingFunction(EaseInExpo);
        case EasingFunctionType::ExpoOut:         return getEasingFunction(EaseOutExpo);
        case EasingFunctionType::ExpoInOut:       return getEasingFunction(EaseInOutExpo);
        case EasingFunctionType::CircIn:          return getEasingFunction(EaseInCirc);
        case EasingFunctionType::CircOut:         return getEasingFunction(EaseOutCirc);
        case EasingFunctionType::CircInOut:       return getEasingFunction(EaseInOutCirc);
        case EasingFunctionType::ElasticIn:       return getEasingFunction(EaseInElastic);
        case EasingFunctionType::ElasticOut:      return getEasingFunction(EaseOutElastic);
		case EasingFunctionType::ElasticHalfOut:  return Functions::ElasticHalfOut;
		case EasingFunctionType::ElasticQuarterOut: return Functions::ElasticQuarterOut;
        case EasingFunctionType::ElasticInOut:    return getEasingFunction(EaseInOutElastic);
        case EasingFunctionType::BackIn:          return getEasingFunction(EaseInBack);
        case EasingFunctionType::BackOut:         return getEasingFunction(EaseOutBack);
        case EasingFunctionType::BackInOut:       return getEasingFunction(EaseInOutBack);
        case EasingFunctionType::BounceIn:        return getEasingFunction(EaseInBounce);
        case EasingFunctionType::BounceOut:       return getEasingFunction(EaseOutBounce);
        case EasingFunctionType::BounceInOut:     return getEasingFunction(EaseInOutBounce);
        }

        return Functions::Linear;
	}
}