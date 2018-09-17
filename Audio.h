#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <SFML\Audio.hpp>

using namespace sf;

class Audio{

public:
    Audio();
	~Audio(void);

	enum AudioType{MenuMusic,GameMusic,PanicMusic};
	void LoadMusic(AudioType audioType);
	void PlayMusic();
	void StopMusic();
    void SetVolume(int a);

private:
	Music music;

};

#endif // AUDIO_H_INCLUDED
