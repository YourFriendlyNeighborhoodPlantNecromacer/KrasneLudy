#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#include "GameState.h"

class StateManager {
private:
    GameState* currentState = nullptr;
    GameState* nextState = nullptr;

    StateManager() {}

public:
    // Dostęp do singletonu
    static StateManager& Instance() {
        static StateManager instance;
        return instance;
    }

    ~StateManager() {
        if (currentState) delete currentState;
        if (nextState) delete nextState;
    }

	// Powiadamia, aby zmienić state w następnej klatce
    void ChangeState(GameState* newState) {
        nextState = newState;
    }

    // Obsluguje zmianę state-ów i usuwa stare w bezpiecznym czasie (następna klatka)
    void ProcessStateChange() {
        if (nextState) {
            if (currentState) delete currentState;
            currentState = nextState;
            nextState = nullptr;
            if (currentState) currentState->Init();
        }
    }

    void Update(float dt) {
        if (currentState) currentState->Update(dt);
    }

    void Draw() {
        if (currentState) currentState->Draw();
    }

	// Nie zezwalać na kopiowanie
    StateManager(const StateManager&) = delete;
    void operator=(const StateManager&) = delete;
};

#endif