
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

class AudioManager
{
public:

	AudioManager();
	~AudioManager() = default;

	void PlayMusic();
	void PauseMusic();
	void ContinueMusic();
	void StopMusic();

	void PlayShotSound();
	void StopShotSound();

	void PlayExplosionSound();



private:
	sf::Music m_Music; // all these audio stuff could go elswhere
	sf::SoundBuffer m_ShotSoundBuffer;
	sf::Sound m_ShotSound;
	sf::SoundBuffer m_ExplosionSoundBuffer;
	sf::Sound m_ExplosionSound;

};







#endif // AUDIO_MANAGER_H
