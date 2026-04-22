#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

class StateManager {
public:
	bool canInput;

	//Create & Destroy
	StateManager();
	~StateManager();

	//State Switching
	void SwitchState();

	//Update & Drawing
	void Update();
	void Draw();
};

#endif