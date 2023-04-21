#include "SoundEffect.h"

SoundEffect SoundEffect::manager;

SoundEffect::SoundEffect() {
	const string cwd = "Resources/Sounds/", ext = ".wav";
	const vector <string> filename = {
		"game_start", "extend", 
		"eat_ghost", "eat_fruit", "eat_pellet",
		"retreating","power_up",
		"pacmanDeath", 
		"spurt_move_1", "spurt_move_2", "spurt_move_3", "spurt_move_4", "spurt_move_5", 
		"credit", "coffee_break"
	};
	buffer.resize(filename.size());
	sound.resize(filename.size());
	for (short i = 0; i < filename.size(); i++) {
		if (buffer[i].loadFromFile(cwd + filename[i] + ext))
			sound[i].setBuffer(buffer[i]);
	}
	
}

SoundEffect& SoundEffect::getManager() {
	return manager;
}

void SoundEffect::playSound(short type, bool isLoop, short volume) {
	sound[type].setVolume(volume);
	sound[type].setLoop(isLoop);
	sound[type].play();
}

bool SoundEffect::isPlaying(short type) const {
	return sound[type].getStatus() == sound[type].Playing;
}

void SoundEffect::stopPlayingSound(short type) {
	if (type == -1) {
		for (int i = 0; i < sound.size(); i++)
			sound[i].stop();
		
		return;
	}
	sound[type].stop();
}

void SoundEffect::pauseSound(short type) {
	if (type == -1) {
		for (int i = 0; i < sound.size(); i++) {
			if (sound[i].getStatus() == sound[i].Playing)
				sound[i].pause();
		}
		return;
	}
	sound[type].pause();
}

void SoundEffect::unpauseSound(short type) {
	if (type == -1) {
		for (int i = 0; i < sound.size(); i++)
			if (sound[i].getStatus() == sound[i].Paused)
				sound[i].play();
		return;
	}
	if (sound[type].getStatus() == sound[type].Paused)
		sound[type].play();
}