
#ifndef TEST_LAYER_4_PERSPCAMTEST
#define TEST_LAYER_4_PERSPCAMTEST

#include <core/Core.h>
#include <core/Layer.h>

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <core/rendering/Shader.h>
#include <core/rendering/drawables/Mesh.h>

class TestLayer4 : public Layer
{
public:
	TestLayer4();
	virtual ~TestLayer4() = default;

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

private:
	float m_ElapsedTime = 0.0f;
	std::shared_ptr<Mesh> m_Tetrahedron = nullptr;
	std::shared_ptr<Mesh> m_Rectangle = nullptr;
	std::shared_ptr<Scene> m_Scene = nullptr;
	Entity m_CameraEntity;
	Entity m_TetrahedronEntity;
	Entity m_RectangleEntity;
};



#endif // TEST_LAYER_4_PERSPCAMTEST
