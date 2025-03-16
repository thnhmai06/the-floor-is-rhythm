#include <iostream>
#include <convert/osu/beatmap.h>

int main(int argc, char *argv[])
{
	CONVERT_OSU_BEATMAP_H::convert(
		"D:\\PROGRAM\\osu!\\Songs\\2287992 Camellia - Operation_ Zenithfall\\Camellia - Operation Zenithfall (Mir) [Final Mission].osu", 
		"D:\\output.tfd"
	);
	return 0;
}