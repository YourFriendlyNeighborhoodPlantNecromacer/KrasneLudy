#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#include "GameState.h"
#include "raylib.h"

class StateManager {
private:
    GameState* currentState = nullptr;
    GameState* nextState = nullptr;

    // Logika przejścia (Transition)
    float transTimer = 0.0f;
    bool transitioning = false;
    bool stateSwapped = false;
    const float transDuration = 1.0f; // Całkowity czas trwania przejścia (sekundy)

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
        if (transitioning) {
            delete newState;
            return;
        }
        nextState = newState;
        transitioning = true;
        transTimer = 0.0f;
        stateSwapped = false;
    }

    // Obsluguje zmianę state-ów i usuwa stare w bezpiecznym czasie (następna klatka)
    void ProcessStateChange() {
        // Wykonaj zamianę stanów w połowie animacji (gdy ekran jest w pełni zakryty)
        if (transitioning && !stateSwapped && transTimer >= transDuration / 2.0f) {
            if (currentState) delete currentState;
            currentState = nextState;
            nextState = nullptr;
            if (currentState) currentState->Init();
            stateSwapped = true;
        }
        // Awaryjna zmiana natychmiastowa, jeśli przejście jest wyłączone
        else if (!transitioning && nextState) {
            if (currentState) delete currentState;
            currentState = nextState;
            nextState = nullptr;
            if (currentState) currentState->Init();
        }
    }

    void Update(float dt) {
        if (transitioning) {
            transTimer += dt;
            if (transTimer >= transDuration) {
                transitioning = false;
                transTimer = 0.0f;
            }
        }
        if (currentState) currentState->Update(dt);
    }

    void Draw() {
        if (currentState) currentState->Draw();

        if (transitioning) {
            float sw = (float)GetScreenWidth();
            float sh = (float)GetScreenHeight();
            float gh = 180.0f; // Wysokość gradientu na krawędzi
            float progress = transTimer / transDuration;

            if (progress <= 0.5f) {
                // Faza 1: Zasłanianie od góry
                float t = progress * 2.0f; // 0.0 -> 1.0
                float y = t * sh;
                DrawRectangle(0, 0, (int)sw, (int)y, BLACK);
                DrawRectangleGradientV(0, (int)y, (int)sw, (int)gh, BLACK, BLANK);
            } else {
                // Faza 2: Odsłanianie (górna krawędź "kurtyny" zjeżdża w dół)
                float t = (progress - 0.5f) * 2.0f; // 0.0 -> 1.0
                float y = t * sh;
                DrawRectangle(0, (int)y, (int)sw, (int)(sh - y), BLACK);
                DrawRectangleGradientV(0, (int)(y - gh), (int)sw, (int)gh, BLANK, BLACK);
            }
        }
    }

	// Nie zezwalać na kopiowanie
    StateManager(const StateManager&) = delete;
    void operator=(const StateManager&) = delete;
};

#endif