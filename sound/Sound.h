#pragma once

#include <SDL_mixer.h>
#include <string>

#include "../utilities/SlotMap.h"

class Sound {
	int channel;

public:
	Mix_Chunk* audio;
	object_id id;
	Sound(std::string file_path);
	Sound();

	~Sound();

	void load_from_file(std::string file_path);

	void play(int loops = 0, int channel = -1);

	double length();
};

template <int n>
using SoundClips = std::array<Sound*, n>;