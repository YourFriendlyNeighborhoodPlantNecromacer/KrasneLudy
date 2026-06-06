#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#include "../../DataStructures/country.h"
#include <raylib.h>

class GameState {
public:
	// Usuwanie stanu
	virtual ~GameState() {};

	// Inicjalizacja tuż po stworzeniu stanu
	virtual void Init() = 0;

	// Aktualizacja (głównie logika) i rysowanie (oddzielnie)
	virtual void Update(float deltaTime, country* countryPtr) = 0;
	virtual void Draw() = 0;
};

#endif
