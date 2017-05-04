#pragma once

#include <SDL_mixer.h>
#include <string>

#include "../utilities/SlotMap.h"

class Sound {
	Mix_Chunk* audio;
	int channel;

public:
	object_id id;
	Sound(std::string file_path);
	Sound();

	~Sound();

	void load_from_file(std::string file_path);

	void play(int loops = 0);
};

template <int n>
using varied_sound = std::array<Sound*, n>;