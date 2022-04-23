
#ifndef InGame_layer2_H
#define InGame_layer2_H


#include <core/Core.h>
#include <core/Layer.h>

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <core/rendering/Shader.h>
#include <core/rendering/DoubleFramebuffer.h>
#include <core/rendering/FramebufferDisplay.h>
#include <core/rendering/Depthbuffer.h>
#include <core/rendering/ImageProcessor.h>
#include <core/rendering/drawables/Mesh.h>

#include <core/rendering/SceneRenderer.h>
#include <core/scene/SceneUpdater.h>

#include <core/GlobalConstants.h>
#include <Game_v1/Common/DualOctTree.h>
#include <Game_v1/Common/EntityManager.h>
#include <Game_v1/Common/AudioManager.h>
#include <Game_v1/Common/Player.h>

#include <SFML/Audio.hpp>


class InGame_layer2 : public Layer
{
public:
	InGame_layer2();
	virtual ~InGame_layer2() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

	virtual void Activate() override;
	virtual void DeActivate() override;

	void ResetLayer();

	entt::entity GetTarget();
	entt::entity GetTarget(const Vec3D& acquisitionLocation, const Vec3D& acquisitionDirection);
	entt::entity GetClosestTarget(const Vec3D& acquisitionLocation, const Vec3D& acquisitionDirection); // -> Move to some other class, maybe Player


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

	void PartialUpdate();

	void HandleUserInput(Timestep ts);
	void ZoomIn();
	void ZoomOut();

	void UpdateScene(Timestep ts);

	void UpdateEnemyShips(Timestep ts);
	void OnEnemyShipDestroyed();
	void OnPickupDestroyed();

	void EvaluateLossCondition();

private:
	float m_ElapsedTime = 0.0f;
	float m_SimulationSpeed = 1.0f;
	float m_ZoomLevel = g_InitialZoomLevel;

	std::shared_ptr<Scene> m_Scene = nullptr;
	SceneRenderer m_SceneRenderer;
	//	SceneUpdater m_SceneUpdater;
	bool m_InFocus = true;

	Player m_Player;
	EntityManager m_EntityManager;
	AudioManager m_AudioManager;

	std::shared_ptr<DualOctTree> m_CollidersOctTree = nullptr;
	// std::shared_ptr<DualOctTree> m_AsteroidsOctTree = nullptr;
	std::shared_ptr<DualOctTree> m_MissillesOctTree = nullptr;
	std::shared_ptr<DualOctTree> m_AntiMissilleOctTree = nullptr;

	FramebufferDisplay m_FbDisplay;
	std::unique_ptr<ImageProcessor> m_ImgProcessor = nullptr;
	bool m_CameraContinuousRotation = false;
	float m_BulletSpawnChance = 0.0f;

	int m_KillCount = 0;
	int m_MaxKillCount = 20;

	bool m_IsLost = false;

};





#endif // InGame_layer2_H
