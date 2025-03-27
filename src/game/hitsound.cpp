#include "game/hitsound.h" // Header
#include "rule/file_format.h"
#include "utilities.h"

//! Hitsound
void Hitsound::Hitsound::read(const std::int32_t HitSound)
{
	normal = Utilities::Math::is_bit_enabled(HitSound, static_cast<std::int32_t>(HitsoundBitmap::NORMAL));
	whistle = Utilities::Math::is_bit_enabled(HitSound, static_cast<std::int32_t>(HitsoundBitmap::WHISTLE));
	finish = Utilities::Math::is_bit_enabled(HitSound, static_cast<std::int32_t>(HitsoundBitmap::FINISH));
	clap = Utilities::Math::is_bit_enabled(HitSound, static_cast<std::int32_t>(HitsoundBitmap::CLAP));
}
std::int32_t Hitsound::Hitsound::to_int() const
{
	std::int32_t hitsound = 0;
	if (normal)
		hitsound |= static_cast<std::int32_t>(HitsoundBitmap::NORMAL);
	if (whistle)
		hitsound |= static_cast<std::int32_t>(HitsoundBitmap::WHISTLE);
	if (finish)
		hitsound |= static_cast<std::int32_t>(HitsoundBitmap::FINISH);
	if (clap)
		hitsound |= static_cast<std::int32_t>(HitsoundBitmap::CLAP);
	return hitsound;
}
Hitsound::Hitsound& Hitsound::Hitsound::operator= (const Parser::Hitsound& hs)
{
	whistle = hs.Whistle;
	finish = hs.Finish;
	clap = hs.Clap;
	return *this;
}

//! Hitsample
Hitsound::HitSample& Hitsound::HitSample::operator= (const Parser::HitObject::HitSample& hs)
{
	normal_set = static_cast<SampleSetType>(static_cast<int32_t>(hs.NormalSet));
	addition_set = static_cast<SampleSetType>(static_cast<int32_t>(hs.AdditionSet));
	index = hs.Index;
	volume = hs.Volume;
	file_name = hs.Filename;
	return *this;
}
Hitsound::HitSample::HitSample(const std::string& hitsample_str)
{
	if (hitsample_str.empty())
		return; // not written
	const auto list = Utilities::String::split(hitsample_str, tfir::HitObjects::HitSample::DELIMETER);
	normal_set = static_cast<SampleSetType>(std::stoi(list[0]));
	addition_set = static_cast<SampleSetType>(std::stoi(list[1]));
	index = std::stoi(list[2]);
	volume = std::stoi(list[3]);
	if (list.size() > 4) file_name = list[4];
}
std::string Hitsound::HitSample::to_string() const
{
	std::string result;
	result.append(std::to_string(static_cast<int32_t>(normal_set)));
	result.push_back(tfir::HitObjects::HitSample::DELIMETER);
	result.append(std::to_string(static_cast<int32_t>(addition_set)));
	result.push_back(tfir::HitObjects::HitSample::DELIMETER);
	result.append(std::to_string(index));
	result.push_back(tfir::HitObjects::HitSample::DELIMETER);
	result.append(std::to_string(volume));
	result.push_back(tfir::HitObjects::HitSample::DELIMETER);
	if (!file_name.empty()) result.append(file_name);
	return result;
}
std::string Hitsound::HitSample::get_hitsound_filename(const HitsoundBitmap& HitsoundType)
{
	if (!file_name.empty())
	{
		// Only provide Normal hitsound
		if (HitsoundType == HitsoundBitmap::NORMAL)
			return file_name;
		return "";
	}

	// sampleSet
	SampleSetType SampleSet = (HitsoundType == HitsoundBitmap::NORMAL) ? normal_set : addition_set;
	std::string SampleSetStr;
	switch (SampleSet)
	{
	case SampleSetType::NORMAL:
		SampleSetStr = "normal";
		break;
	case SampleSetType::SOFT:
		SampleSetStr = "soft";
		break;
	case SampleSetType::DRUM:
		SampleSetStr = "drum";
		break;
	default:
		return ""; // No custom sampleSet, use skin hitsound instand
	}

	// hitSound
	std::string HitsoundTypeStr;
	switch (HitsoundType)
	{
	case HitsoundBitmap::WHISTLE:
		HitsoundTypeStr = "whistle";
		break;
	case HitsoundBitmap::FINISH:
		HitsoundTypeStr = "finish";
		break;
	case HitsoundBitmap::CLAP:
		HitsoundTypeStr = "clap";
		break;
	default:
		HitsoundTypeStr = "normal";
		break;
	}

	std::string result;
	result.append(SampleSetStr);
	result.append("-hit");
	result.append(HitsoundTypeStr);
	if (index != 0 && index != 1)
		result.append(std::to_string(index));
	result.append(".wav");
	return result;
}