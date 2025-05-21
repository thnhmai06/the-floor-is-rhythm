#pragma once
#include "utilities.h"

namespace Engine
{
	template <typename NumberType>
	struct PercentValue
	{
	protected:
		NumberType min;
		NumberType max;

	public:
		double percent = 1; // TODO: tránh phụ thuộc
		const PercentValue* parent = nullptr;

		[[nodiscard]] NumberType get_value(const bool follow_parent = true, std::optional<double> percent = std::nullopt) const
		{
			const auto parent_percent = (follow_parent && parent) ? parent->get_percent(true) : 1.0;
			return Utilities::Math::to_value(percent.value_or(this->percent) * parent_percent, min, max, false);
		}
		[[nodiscard]] double get_percent(const bool follow_parent = true, std::optional<NumberType> value = std::nullopt) const
		{
			const auto parent_percent = (follow_parent && parent) ? parent->get_percent(true) : 1.0;
			if (Utilities::Math::is_equal(parent_percent, 0.0)) return 0;
			if (!value.has_value())
				return percent * parent_percent;
			return Utilities::Math::to_percent(value.value(), min, max, false) / parent_percent;
		}
		double set_value(const NumberType& value, const bool clamp = true) const
		{
			return percent = Utilities::Math::to_percent(value, min, max, clamp);
		}
		[[nodiscard]] std::pair<NumberType, NumberType> get_range() const
		{
			return { min, max };
		}
		double set_range(const NumberType& min, const NumberType& max, const bool static_value = false)
		{
			if (static_value)
				percent = Utilities::Math::to_percent(get_value(false), min, max, false);
			this->min = min;
			this->max = max;

			return percent;
		}

		PercentValue(const NumberType& min, const NumberType& max, const double percent = 1.0, const PercentValue* parent = nullptr)
			: min(min), max(max), parent(parent), percent(percent)
		{
		}
	};
}