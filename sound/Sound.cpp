#include "Sound.h"



Sound::Sound(std::string file_path)
	: channel(0) {
	load_from_file(file_path);
}

Sound::Sound() {}

Sound::~Sound() {
	Mix_FreeChunk(audio);
}

void Sound::load_from_file(std::string file_path)
{
	audio = Mix_LoadWAV(file_path.c_str());
	if (audio == NULL)
	{
		fprintf(stderr, Mix_GetError(), file_path.c_str());
	}
}

void Sound::play(int loops) {
	channel = Mix_PlayChannel(-1, audio, loops);
}

double Sound::length()
{
	return ((double)audio->alen / 176.4);
}
