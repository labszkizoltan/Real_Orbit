
#ifndef TEST_LAYER_5_H
#define TEST_LAYER_5_H

#include <core/Core.h>
#include <core/Layer.h>

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <core/rendering/Shader.h>
#include <core/rendering/Framebuffer.h>
#include <core/rendering/Depthbuffer.h>
#include <core/rendering/drawables/Mesh.h>

class TestLayer5 : public Layer
{
public:
	TestLayer5();
	virtual ~TestLayer5() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

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
	std::shared_ptr<Mesh> m_Tetrahedron = nullptr;
	std::shared_ptr<Mesh> m_Rectangle = nullptr;
	std::shared_ptr<Mesh> m_Cube = nullptr;
	std::shared_ptr<Mesh> m_Textured  = nullptr;
	std::shared_ptr<Mesh> m_Skybox = nullptr;

	std::shared_ptr<Scene> m_Scene = nullptr;
	Entity m_CameraEntity;
	Entity m_TetrahedronEntity;
	Entity m_RectangleEntity;
	Entity m_CubeEntity;
	Entity m_TexturedEntity;
	Entity m_SkyboxEntity;
};



#endif // TEST_LAYER_5_H
