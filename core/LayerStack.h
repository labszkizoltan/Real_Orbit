
#ifndef LAYER_STACK_H
#define LAYER_STACK_H

#include <vector>

#include "core/Layer.h"

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	void OnUpdate(Timestep t);
	void OnEvent(Event& e);

	/*
	// I don like having 8 functions just so I can loop over the layers once in the application functions
	std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
	std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

	std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
	std::vector<Layer*>::const_iterator end()   const { return m_Layers.end(); }
	std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
	std::vector<Layer*>::const_reverse_iterator rend()   const { return m_Layers.rend(); }
	*/

private:
	std::vector<Layer*> m_Layers;
	unsigned int m_LayerInsertIndex = 0;
};





#endif // LAYER_STACK_H
