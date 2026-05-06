#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#include <raylib.h>

class GameState {
public:
	// Usuwanie
	virtual ~GameState() {};

	// Tworzenie tuż po tworzeniu state-a
	virtual void Init() = 0;

	// Aktualizacja (głównie do logiki) i rysowanie (osobno)
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
};

#endif