
// I test the rebuilt functionalities after switching to instance rendering

#ifndef TEST_LAYER_8_H
#define TEST_LAYER_8_H

#include <core/Core.h>
#include <core/Layer.h>

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <core/rendering/Shader.h>
//#include <core/rendering/Framebuffer.h>
#include <core/rendering/DoubleFramebuffer.h>
#include <core/rendering/FramebufferDisplay.h>
#include <core/rendering/Depthbuffer.h>
#include <core/rendering/ImageProcessor.h>
#include <core/rendering/drawables/Mesh.h>

#include <core/rendering/SceneRenderer.h>
#include <core/scene/SceneUpdater.h>

#include <core/GlobalConstants.h>


#include <SFML/Audio.hpp>

class TestLayer8 : public Layer
{
public:
	TestLayer8();
	virtual ~TestLayer8() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

	void RandomRocketLaunch(int meshIdx, Vec3D origin);
	entt::entity GetTarget();
	void EmitMesh(int meshIdx, TransformComponent transform);
	void SpawnAsteroid();
	void LaunchMissile(int meshIdx, TransformComponent transform, entt::entity target);
	void RemoveMesh(int meshIdx);

private:
	bool OnWindowResize(Event& e);
	bool OnLoosingFocus(Event& e);
	bool OnGainingFocus(Event& e);
	bool OnTextEntered(Event& e);
	bool OnKeyPressed(Event& e);
	bool OnKeyReleased(Event& e);
	bool MouseWheelScrolled(Event& e);
	bool OnMouseButtonPressed(Event& e);
	bool OnMouseButtonReleased(Event& e);
	bool OnMouseMoved(Event& e);
	bool OnMouseEntered(Event& e);
	bool OnMouseLeft(Event& e);

	void HandleUserInput(Timestep ts);
	void ZoomIn();
	void ZoomOut();

private:
	float m_ElapsedTime = 0.0f;
	float m_SimulationSpeed = 1.0f;
	float m_ZoomLevel = g_InitialZoomLevel;

	std::shared_ptr<Scene> m_Scene = nullptr;
	SceneRenderer m_SceneRenderer;
	SceneUpdater m_SceneUpdater;
	bool m_InFocus = true;

	sf::Music m_Music;
	sf::SoundBuffer m_ShotSoundBuffer;
	sf::Sound m_ShotSound;
	sf::Sound m_ExplosionSound;


//	std::shared_ptr<Shader> m_ScreenDrawer = nullptr;

	FramebufferDisplay m_FbDisplay;
	std::unique_ptr<ImageProcessor> m_ImgProcessor = nullptr;
	int m_current_slot = 0;
	int m_mipmap_level = 0;
	bool m_CameraContinuousRotation = false;

};





#endif // TEST_LAYER_8_H
