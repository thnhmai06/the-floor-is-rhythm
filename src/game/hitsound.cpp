#include "game/hitsound.h" // Header
#include <osu!parser/Parser/Structures/Beatmap/HitObject.hpp>

//! Hitsound
Hitsound& Hitsound::operator= (const Parser::Hitsound& hs)
{
	Whistle = hs.Whistle;
	Finish = hs.Finish;
	Clap = hs.Clap;

	return *this;
}

//! Hitsample
HitSample& HitSample::operator= (const Parser::HitObject::HitSample& hs)
{
	NormalSet = hs.NormalSet;
	AdditionSet = hs.AdditionSet;
	Index = hs.Index;
	Volume = hs.Volume;
	Filename = hs.Filename;

	return *this;
}