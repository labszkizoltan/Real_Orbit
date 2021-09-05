
#ifndef TEST_LAYER_2_H
#define TEST_LAYER_2_H

#include <core/Core.h>
#include <core/Layer.h>

class TestLayer2 : public Layer
{
public:
	TestLayer2();
	virtual ~TestLayer2() = default;

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

	

};



#endif // TEST_LAYER_2_H
