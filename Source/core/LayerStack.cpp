#include "LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
	// Return value of std::find : Iterator to the first element satisfying the condition or last if no such element is found.
	// auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	//auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
		m_Layers.erase(it);
}


void LayerStack::OnUpdate(Timestep t)
{
	for (int i = 0; i < m_Layers.size(); i++)
	{
		if(m_Layers[i]->IsActive())
			m_Layers[i]->OnUpdate(t);
	}
}

void LayerStack::OnEvent(Event& e)
{
	for (int i = 0; i < m_Layers.size(); i++)
	{
		if (e.IsHandled())
			return;
		
		if (m_Layers[i]->IsActive())
			m_Layers[i]->OnEvent(e);
	}
}

void LayerStack::ActivateLayer(int idx)
{
	if (idx >= 0 && idx < m_Layers.size())
		m_Layers[idx]->Activate();
}

/*
Layer* LayerStack::GetActiveLayer()
{
	return m_Layers[m_ActiveIndex];
}

void LayerStack::SetActiveLayer(int idx)
{
	if (idx >= 0 && idx < m_Layers.size())
		m_ActiveIndex = idx;
}
*/



