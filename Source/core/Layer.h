
#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <core/ROCore.h>

class Layer
{
public:
	Layer(const std::string& name = "LAYER");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(Timestep ts) {}
	virtual void OnEvent(Event& event) {}

	inline const std::string& GetName() const { return m_DebugName; }

protected:
	std::string m_DebugName;
};


#endif LAYER_H // LAYER_H
