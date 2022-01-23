
#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <core/Core.h>

class Layer
{
public:
	Layer(const std::string& name = "LAYER");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(Timestep ts) {}
	virtual void OnEvent(Event& event) {}

	virtual void Activate() { m_IsActive = true; };
	virtual void DeActivate() { m_IsActive = false; };
	virtual bool IsActive() { return m_IsActive; };

	inline const std::string& GetName() const { return m_DebugName; }

protected:
	std::string m_DebugName;
	bool m_IsActive = true;
};


#endif LAYER_H // LAYER_H
