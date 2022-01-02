
#ifndef SCENE_UPDATER_H
#define SCENE_UPDATER_H

#include "Scene.h"


#include <SFML/Audio.hpp> // just quickly hacking in explosion sounds

class SceneUpdater
{
public:

	SceneUpdater();
	SceneUpdater(std::shared_ptr<Scene> scene);
	~SceneUpdater();

	void UpdateScene(Timestep ts);
	void SetScene(std::shared_ptr<Scene> scene) { m_Scene = scene; }
	std::shared_ptr<Scene> GetScene() { return m_Scene; }

private:
	void SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn);

private:
	std::shared_ptr<Scene> m_Scene = nullptr;

	// just quickly hacking in explosion sounds:
	sf::SoundBuffer m_ExplosionSoundBuffer;
	sf::Sound m_ExplosionSound;


//protected:
//
//	static entt::registry& GetRegistry(Scene scene)
//	{
//		return scene.m_Registry;
//	}

};


#endif // SCENE_UPDATER_H
