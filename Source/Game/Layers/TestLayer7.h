
// in this layer I'd like to test the scene renderer, and perhaps the scene serializer

#ifndef TEST_LAYER_7_H
#define TEST_LAYER_7_H

#include <core/Core.h>
#include <core/Layer.h>

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <core/rendering/Shader.h>
#include <core/rendering/Framebuffer.h>
#include <core/rendering/Depthbuffer.h>
#include <core/rendering/drawables/Mesh.h>

// #include <core/rendering/SceneRenderer.h>
// #include <core/scene/SceneUpdater.h>

class TestLayer7 : public Layer
{
public:
	TestLayer7();
	virtual ~TestLayer7() = default;

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

//	std::shared_ptr<Scene> m_Scene = nullptr;
//	SceneRenderer m_SceneRenderer;
//	SceneUpdater m_SceneUpdater;

	std::shared_ptr<Shader> m_IShader = nullptr;
	std::shared_ptr<Mesh> m_IMesh = nullptr;
	std::vector<TransformComponent> m_Transforms;

};



#endif // TEST_LAYER_7_H
