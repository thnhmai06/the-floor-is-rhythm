#ifndef OSU_H
#define OSU_H

#include <fstream>
#include "beatmap.h"

static General convert_general(std::ifstream&, std::string&);
static Difficulty convert_difficulty(std::ifstream&, std::string&);
static Metadata convert_metadata(std::ifstream&, std::string&);

#endif
