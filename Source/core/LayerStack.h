
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

	void ActivateLayer(int idx);

	/*
	Layer* GetActiveLayer();
	void SetActiveLayer(int idx);
	*/


private:
	std::vector<Layer*> m_Layers;
//	int m_ActiveIndex = 0; // index of the currently active layer, only one layer can be active at a time
	unsigned int m_LayerInsertIndex = 0;

};





#endif // LAYER_STACK_H
