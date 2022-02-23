
#ifndef INGAME_LAYER_H
#define INGAME_LAYER_H


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


class InGame_layer : public Layer
{
public:
	InGame_layer();
	virtual ~InGame_layer() = default;

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

	void BuildOctTree(); // -> this may need to move to the EntityManager as well, not 100% sure

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

	void UpdateScene(Timestep ts);

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

	std::unique_ptr<DualOctTree> m_AsteroidOctTree = nullptr;


	FramebufferDisplay m_FbDisplay;
	std::unique_ptr<ImageProcessor> m_ImgProcessor = nullptr;
	bool m_CameraContinuousRotation = false;
	float m_BulletSpawnChance = 0.0f;

	int m_EarthHitCount = 0;
	int m_MaxEarthHitCount = 5;

};



template <typename UserDataType>
struct targeting_data
{
	float dot_product;
	UserDataType user_data;

	bool operator<(const targeting_data<UserDataType>& other) const
	{
		return dot_product < other.dot_product; // this is the original
//		return dot_product > other.dot_product;
	}
};

template <typename UserDataType>
std::ostream& operator<<(std::ostream& stream, const targeting_data<UserDataType>& td)
{
	stream << td.dot_product << "\t" << td.user_data << "\n";
	return stream;
}





#endif // INGAME_LAYER_H
