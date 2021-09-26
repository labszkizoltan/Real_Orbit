
#ifndef TEST_LAYER_H
#define TEST_LAYER_H

#include <core/ROCore.h>
#include <core/Layer.h>

class TestLayer : public Layer
{
public:
	TestLayer();
	virtual ~TestLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

private:
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
	unsigned int m_ShaderProgram = 0;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;
	unsigned int m_VAO_square = 0;
	unsigned int m_VBO_square = 0;
	unsigned int m_EBO_square = 0;

	float m_TimeElapsed = 0.0f;

};



#endif // TEST_LAYER_H
