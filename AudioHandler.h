#pragma once

#include <irrKlang.h>

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;


class AudioHandler 
{
public:
	AudioHandler();
	~AudioHandler();

	void ambientSound(ik_c8* path, bool looping = true);
	void ambientPlay(bool play);

	void reInit();

private:
	ISoundEngine*	soundEngine;
	ISound*			bgSound;
};

extern AudioHandler* gAudio;