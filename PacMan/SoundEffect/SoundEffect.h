#pragma once
#include "SFML/Audio.hpp"
#include <vector>
using namespace std;
using namespace sf;

#define SOUND_GAMESTART 0
#define SOUND_EXTEND 1
#define SOUND_EATGHOST 2
#define SOUND_EATFRUIT 3
#define SOUND_EATPELLET 4
#define SOUND_RETREAT 5
#define SOUND_POWERUP 6
#define SOUND_PACMAN_DEATH 7
#define SOUND_SPURT_MOVE_1 8
#define SOUND_SPURT_MOVE_2 9
#define SOUND_SPURT_MOVE_3 10
#define SOUND_SPURT_MOVE_4 11
#define SOUND_SPURT_MOVE_5 12
#define SOUND_CREDIT 13
#define SOUND_COFFEEBREAK 14

class SoundEffect
{
private:
	vector <SoundBuffer> buffer;
	vector <Sound> sound;

	static SoundEffect manager;
	SoundEffect();
public:
	static SoundEffect& getManager();
	void playSound(short type, bool isLoop, short volume = 100);
	bool isPlaying(short type) const;
	void stopPlayingSound(short type = -1);
	void pauseSound(short type = -1);
	void unpauseSound(short type = -1);
};

