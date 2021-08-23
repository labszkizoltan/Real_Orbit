
#ifndef TEST_LAYER_H
#define TEST_LAYER_H

#include <core/Core.h>
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


};



#endif // TEST_LAYER_H
