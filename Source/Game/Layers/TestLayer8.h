
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
#include <core/rendering/drawables/Mesh.h>

#include <core/rendering/SceneRenderer.h>
#include <core/scene/SceneUpdater.h>

#include <core/GlobalConstants.h>

class TestLayer8 : public Layer
{
public:
	TestLayer8();
	virtual ~TestLayer8() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

	entt::entity GetTarget();
	void EmitMesh(int meshIdx, TransformComponent transform);
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

private:
	float m_ElapsedTime = 0.0f;
	float m_SimulationSpeed = 1.0f;
	float m_ZoomLevel = g_InitialZoomLevel;

	std::shared_ptr<Scene> m_Scene = nullptr;
	SceneRenderer m_SceneRenderer;
	SceneUpdater m_SceneUpdater;
	bool m_InFocus = true;

//	std::shared_ptr<Shader> m_ScreenDrawer = nullptr;

	FramebufferDisplay m_FbDisplay;

};



#endif // TEST_LAYER_8_H
