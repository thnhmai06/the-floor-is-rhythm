#ifndef GAME_HITSOUND_H
#define GAME_HITSOUND_H

struct Hitsound
{
	struct Type
	{
		const int WHISTLE = 2;
		const int FINISH = 4;
		const int CLAP = 8;

		int value = 0;
		bool whistle = false;
		bool finish = false;
		bool clap = false;

		void apply(int v)
		{
			value = v;
			if (v > CLAP)
			{
				clap = true;
				v -= CLAP;
			}
			if (v > FINISH) {
				finish = true;
				v -= FINISH;
			}
			if (v > WHISTLE)
			{
				whistle = true;
				v -= WHISTLE;
			}
		}

		Type() = default;
		Type(const int value) { apply(value); }
	};
};

#endif
