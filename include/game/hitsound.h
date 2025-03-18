#pragma once

#include <osu!parser/Parser/Structures/Beatmap/HitObject.hpp>
// lưu trữ Hitsound và Hitsample dựa trên cấu trúc của osu!
// (mình là người viết ra mấy đống này mà xD)


struct Hitsound : Parser::Hitsound
{
	Hitsound& operator= (const Parser::Hitsound& hs)
	{
		Whistle = hs.Whistle;
		Finish = hs.Finish;
		Clap = hs.Clap;

		return *this;
	}
};
struct HitSample : Parser::HitObject::HitSample
{
	HitSample& operator= (const Parser::HitObject::HitSample& hs)
	{
		NormalSet = hs.NormalSet;
		AdditionSet = hs.AdditionSet;
		Index = hs.Index;
		Volume = hs.Volume;
		Filename = hs.Filename;

		return *this;
	}
};