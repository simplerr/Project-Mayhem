

#include "AudioHandler.h"
#include "d3dUtil.h"


AudioHandler::AudioHandler() 
{
	soundEngine = createIrrKlangDevice();
	if(!soundEngine)
		MessageBox(0, "Error starting sound device!", 0, 0);
}

AudioHandler::~AudioHandler() 
{
	soundEngine->drop();
}

void AudioHandler::ambientSound(ik_c8* path, bool looping)
{
	ISoundSource* srcs = soundEngine->addSoundSourceFromFile(path);
	bgSound = soundEngine->play2D(srcs, looping);
	delete srcs;
}

void AudioHandler::ambientPlay(bool play)
{
	bgSound->setIsPaused(play);
}

void AudioHandler::reInit() 
{
	bgSound->drop();
}