
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

void AudioManager::StopShotSound()
{
	m_ShotSound.stop();
}


void AudioManager::PlayExplosionSound()
{
	m_ExplosionSound.play();
}
