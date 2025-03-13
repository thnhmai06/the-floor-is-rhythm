#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game/hitsound.h"

const int DIRECTION_RIGHT = 0;
const int DIRECTION_UP = 1;
const int DIRECTION_DOWN = 2;
const int DIRECTION_LEFT = 3;

struct Note
{

	const int type = 1;
	int time;
	int direction;
	Hitsound hitsound;
};

struct Slider
{
	const int type = 2;
	int time;
	int direction;
	Hitsound hitsound;
};

#endif
