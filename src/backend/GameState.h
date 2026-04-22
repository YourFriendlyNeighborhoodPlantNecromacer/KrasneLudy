#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#include <raylib.h>

class GameState {
public:
	//Destroy
	virtual ~GameState() {};

	//Creation
	virtual void Init() = 0;

	//Update & Drawing
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
};

#endif