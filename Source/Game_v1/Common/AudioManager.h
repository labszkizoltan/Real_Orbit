
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

class AudioManager
{
public:

	AudioManager();
	AudioManager(const std::string& intro_speech_path);
	~AudioManager() = default;

	void SetVolume(float volume);

	void PlayMusic();
	void PauseMusic();
	void ContinueMusic();
	void StopMusic();

	void PlayIntroSpeech();
	void StopIntroSpeech();

	void PlayShotSound();
	void StopShotSound();

	void PlayExplosionSound();

	void SetIntroSpeech(const std::string& intro_speech_path);


public:
	bool m_IntroPlayed = false;

private:
	sf::Music m_Music; // all these audio stuff could go elswhere
	sf::SoundBuffer m_IntroSpeechBuffer;
	sf::Sound m_IntroSpeechSound;
	sf::SoundBuffer m_ShotSoundBuffer;
	sf::Sound m_ShotSound;
	sf::SoundBuffer m_ExplosionSoundBuffer;
	sf::Sound m_ExplosionSound;

};







#endif // AUDIO_MANAGER_H
