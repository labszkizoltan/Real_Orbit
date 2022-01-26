
#ifndef MENU_STATE_MANAGER_H
#define MENU_STATE_MANAGER_H

#include <core/scene/CoreComponents.h>
#include <vector>

using ActivationFn = std::function<void()>;

struct MenuState
{
	TransformComponent camera_location;
	// MenuStates will be put into a vector, and these are the indices of the
	// parent, next/prev, child nodes
	int parent_state = -1;
	int next_state = -1;
	int previous_state = -1;
	int child_state = -1;
	ActivationFn activation_function = []() { 
		std::cout << "default activation was called\n";
	};
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

	void ActivateState();

	bool ValidIndex(int stateIdx);


public:
	void* m_MenuLayer = nullptr;
	int m_CurrentState = -1;
	std::vector<MenuState> m_States;
};













#endif MENU_STATE_MANAGER_H
