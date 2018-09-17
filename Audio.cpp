#include "Audio.h"
#include <string>

using namespace std;

Audio::Audio()
{
}

Audio::~Audio(void)
{
}

void Audio::LoadMusic(AudioType audioType)
{
    string path;

    if(audioType==MenuMusic)
    {
	path="data/Dead_Silence.ogg";
    }
    else if(audioType==GameMusic)
    {
    path="data/graveyardmoon.ogg";
    }
    else if(audioType==PanicMusic)
    {
    path="data/Panic_and_Paranoia.ogg";
    }
	music.openFromFile(path);
	music.setLoop(true);
}

void Audio::PlayMusic()
{
	music.play();

}
void Audio::StopMusic()
{
	music.stop();
}

