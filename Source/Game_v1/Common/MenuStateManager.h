
#ifndef MENU_STATE_MANAGER_H
#define MENU_STATE_MANAGER_H

#include <core/scene/CoreComponents.h>
//#include <Game_v1/Layers/01_MenuLayer.h>

#include <vector>

enum class MenuStateType { MAIN_MENU, LEVEL_SELECT, OPTIONS };

struct MenuState
{
	TransformComponent camera_location;
	MenuStateType state_type;
	// MenuStates will be put into a vector, and these are the indices of the
	// parent, next/prev, child nodes
	int parent_state = -1;
	int next_state = -1;
	int previous_state = -1;
	int child_state = -1;
};


class StateManager
{
public:
	StateManager();
	~StateManager() = default;

	void SetState(int stateIdx);
	int GetParentState();
	int GetNextState();
	int GetPreviousState();
	int GetChildState();

	bool ValidIndex(int stateIdx);


public:
//	Menu_layer* m_MenuLayer = nullptr;
	void* m_MenuLayer = nullptr;
	int m_CurrentState = -1;
	std::vector<MenuState> m_States;
};













#endif MENU_STATE_MANAGER_H
