
#ifndef MARS_MISSION_LAYER_H
#define MARS_MISSION_LAYER_H


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
#include <Game_v1/Common/UnitControl.h>

#include <SFML/Audio.hpp>


class MarsMission_layer : public Layer
{
public:
	MarsMission_layer();
	virtual ~MarsMission_layer() = default;

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

	void PartialUpdate(Timestep ts);

	void HandleUserInput(Timestep ts);
	void ZoomIn();
	void ZoomOut();

	void UpdateScene(Timestep ts);

	void UpdateShips(Timestep ts);
	void AddWaypoints();
	void UpdateControlPoints(Timestep ts);

	void SpawnShips(int count, int team, Vec3D location, float range);
	void SpawnCapitalShips(int team);
	void SpawnShips_2(Timestep ts);
	void SpawnCapitalShips_2(int team);

	void DrawCommandPoints();

	void OnTeam0ShipDestroyed();
	void OnTeam1ShipDestroyed();

	void OnVictoryComponentDestroyed();
	void OnLoosingComponentDestroyed();

	void EvaluateLossCondition();

private:
	float m_ElapsedTime = 0.0f;
	float m_SimulationSpeed = 0.2f;
	float m_ZoomLevel = g_InitialZoomLevel;

	std::shared_ptr<Scene> m_Scene = nullptr;
	SceneRenderer m_SceneRenderer;
	std::shared_ptr<Scene> m_OtherScene = nullptr;
	SceneRenderer m_OtherSceneRenderer;
	bool m_InFocus = true;

	int m_DisplayedSlot = 0;

	Player m_Player;
	EntityManager m_EntityManager;
	AudioManager m_AudioManager; // ("assets/audio/MoonMission_introSpeech.wav");
	UnitController m_UnitController;

	std::shared_ptr<DualOctTree> m_CollidersOctTree = nullptr;
	std::shared_ptr<DualOctTree> m_MissillesOctTree = nullptr;
	std::shared_ptr<DualOctTree> m_AntiMissilleOctTree = nullptr;

	std::shared_ptr<DualOctTree> m_Team0_Tree = nullptr;
	std::shared_ptr<DualOctTree> m_Team1_Tree = nullptr;
	std::shared_ptr<DualOctTree> m_Team0_missilles_Tree = nullptr;
	std::shared_ptr<DualOctTree> m_Team1_missilles_Tree = nullptr;


	FramebufferDisplay m_FbDisplay;
	std::unique_ptr<ImageProcessor> m_ImgProcessor = nullptr;
	bool m_CameraContinuousRotation = false;

	int m_Team0_kill_counter = 0;
	int m_Team1_kill_counter = 0;
	int m_MaxKillCount = 500;

	int m_Team0_blockade = 0;
	int m_Team1_blockade = 0;
	int m_MaxBlockadeCount = 100;

	entt::entity m_PlayersTarget = entt::tombstone;
	entt::entity m_LockedTarget = entt::tombstone;

	bool m_Victory = false;
	bool m_IsLost = false;

	friend class UnitController;

};





#endif // MARS_MISSION_LAYER_H
