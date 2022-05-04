
#include "AudioManager.h"

#include <string>
#include <core/Application.h>

const std::string BackgroundMusicPath = "assets/audio/Adrift_by_Hayden_Folker.ogg";
const std::string ShotSoundPath       = "assets/audio/SciFi_weapon_MultiShot_1.wav";
const std::string ExplosionSoundPath  = "assets/audio/Explosion_big.wav";

AudioManager::AudioManager()
{
	// load background music
	if (!m_Music.openFromFile(BackgroundMusicPath))
		LOG_ERROR("Audio not found: " + BackgroundMusicPath);

	// Load a sound to play
	if (!m_ShotSoundBuffer.loadFromFile(ShotSoundPath))
		LOG_ERROR("Audio not found: " + ShotSoundPath);
	m_ShotSound.setBuffer(m_ShotSoundBuffer);
	m_ShotSound.setVolume(60.0f);

	// Load a sound to play
	if (!m_ExplosionSoundBuffer.loadFromFile(ExplosionSoundPath))
		LOG_ERROR("SceneUpdater() - Audio not found: " + ExplosionSoundPath);
	m_ExplosionSound.setBuffer(m_ExplosionSoundBuffer);
	m_ExplosionSound.setVolume(60.0f);
}

AudioManager::AudioManager(const std::string& intro_speech_path)
{
	// load background music
	if (!m_Music.openFromFile(BackgroundMusicPath))
		LOG_ERROR("Audio not found: " + BackgroundMusicPath);

	// Load a sound to play
	if (!m_IntroSpeechBuffer.loadFromFile(intro_speech_path))
		LOG_ERROR("Audio not found: " + intro_speech_path);
	m_IntroSpeechSound.setBuffer(m_IntroSpeechBuffer);
	m_IntroSpeechSound.setVolume(60.0f);

	// Load a sound to play
	if (!m_ShotSoundBuffer.loadFromFile(ShotSoundPath))
		LOG_ERROR("Audio not found: " + ShotSoundPath);
	m_ShotSound.setBuffer(m_ShotSoundBuffer);
	m_ShotSound.setVolume(60.0f);

	// Load a sound to play
	if (!m_ExplosionSoundBuffer.loadFromFile(ExplosionSoundPath))
		LOG_ERROR("SceneUpdater() - Audio not found: " + ExplosionSoundPath);
	m_ExplosionSound.setBuffer(m_ExplosionSoundBuffer);
	m_ExplosionSound.setVolume(60.0f);
}

void AudioManager::SetVolume(float volume)
{
	m_Music.setVolume(volume);
	m_ShotSound.setVolume(volume);
	m_ExplosionSound.setVolume(volume);
	m_IntroSpeechSound.setVolume(volume);
}

void AudioManager::PlayMusic()
{
	// if music finished, start again
	if (m_Music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		m_Music.openFromFile(BackgroundMusicPath);
		m_Music.play();
	}
}

void AudioManager::PauseMusic()
{
	m_Music.pause();
}

void AudioManager::ContinueMusic()
{
	m_Music.play();
}

void AudioManager::StopMusic()
{
	m_Music.stop();
}

void AudioManager::PlayShotSound()
{
	if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) {
		m_ShotSound.play();
	}
}

void AudioManager::SetIntroSpeech(const std::string& intro_speech_path)
{
	// Load a sound to play
	if (!m_IntroSpeechBuffer.loadFromFile(intro_speech_path))
		LOG_ERROR("Audio not found: " + intro_speech_path);
	m_IntroSpeechSound.setBuffer(m_IntroSpeechBuffer);
	m_IntroSpeechSound.setVolume(100.0f);
}

void AudioManager::StopShotSound()
{
	m_ShotSound.stop();
}


void AudioManager::PlayExplosionSound()
{
	m_ExplosionSound.play();
}

void AudioManager::PlayIntroSpeech()
{
	if (!m_IntroPlayed)
	{
		m_IntroSpeechSound.play();
		m_IntroPlayed = true;
	}
}

void AudioManager::StopIntroSpeech()
{
	m_IntroSpeechSound.stop();
}
