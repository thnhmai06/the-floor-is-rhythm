#pragma once
#include <osu!parser/Parser/Structures/Beatmap/HitObject.hpp>
// lưu trữ Hitsound và Hitsample dựa trên cấu trúc của osu!
// (mình là người viết ra mấy đống này mà xD)


struct Hitsound : Parser::Hitsound
{
	Hitsound& operator= (const Parser::Hitsound& hs);
};
struct HitSample : Parser::HitObject::HitSample
{
	HitSample& operator= (const Parser::HitObject::HitSample& hs);
};