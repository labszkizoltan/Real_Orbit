
#include "MenuStateManager.h"
#include <Game_v1/Layers/01_MenuLayer.h>

StateManager::StateManager()
	: m_MenuLayer(nullptr), m_CurrentState(-1), m_States(std::vector<MenuState>())
{
}

void StateManager::SetState(int stateIdx)
{
	if (!(0 <= stateIdx && stateIdx < m_States.size()))
		return;
	
	m_CurrentState = stateIdx;
	TransformComponent& camTrf = ((Menu_layer*)(m_MenuLayer))->m_Scene->GetCamera();
	camTrf = m_States[m_CurrentState].camera_location;
}

int StateManager::GetParentState()
{
	if (ValidIndex(m_CurrentState))
		return m_States[m_CurrentState].parent_state;
	return -1;
}

int StateManager::GetNextState()
{
	if (ValidIndex(m_CurrentState))
		return m_States[m_CurrentState].next_state;
	return -1;
}

int StateManager::GetPreviousState()
{
	if (ValidIndex(m_CurrentState))
		return m_States[m_CurrentState].previous_state;
	return -1;
}

int StateManager::GetChildState()
{
	if (ValidIndex(m_CurrentState))
		return m_States[m_CurrentState].child_state;
	return -1;
}


// m_CurrentState should always be a valid index, so eventually these validity checks shouldnt be necessary
bool StateManager::ValidIndex(int stateIdx)
{
	return (0 <= stateIdx) && (stateIdx < m_States.size());
}
