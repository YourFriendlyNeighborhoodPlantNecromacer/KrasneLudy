#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#include "GameState.h"
#include "raylib.h"
#include "../../DataStructures/country.h"

class StateManager {
private:
    GameState* currentState = nullptr;
    GameState* nextState = nullptr;

    // Logika przejścia
    float transitionTimer = 0.0f;
    bool transitioning = false;
    bool stateSwapped = false;

    static constexpr float TRANSITION_DURATION = 1.0f;
    static constexpr float GRADIENT_HEIGHT = 180.0f;
    static constexpr float MID_TRANSITION_THRESHOLD = 0.5f;
    static constexpr float ANIMATION_SPEED_SCALE = 2.5f;
    static constexpr float PHASE_OFFSET = -0.5f;

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

    // Manualne wyczyszczenie zanim wyłączy się silnik Raylib
    void Shutdown() {
        if (currentState) {
            delete currentState;
            currentState = nullptr;
        }
        if (nextState) {
            delete nextState;
            nextState = nullptr;
        }
    }

    bool IsTransitioning() const { return transitioning; }

	// Powiadamia o zmianie stanu w następnej klatce
    void ChangeState(GameState* newState) {
        if (transitioning || newState == nullptr) {
            if (newState) delete newState;
            return;
        }
        nextState = newState;
        transitioning = true;
        transitionTimer = 0.0f;
        stateSwapped = false;
    }

    // Obsługuje zmianę stanów i usuwa stare w bezpiecznym czasie (następna klatka)
    void ProcessStateChange() {
        // Wykonaj zamianę stanów w połowie animacji (gdy ekran jest w pełni zakryty)
        if (transitioning && !stateSwapped && transitionTimer >= TRANSITION_DURATION * MID_TRANSITION_THRESHOLD) {
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

    void Update(float deltaTime, country* countryPtr) {
        if (transitioning) {
            transitionTimer += deltaTime;
            if (transitionTimer >= TRANSITION_DURATION) {
                transitioning = false;
                transitionTimer = 0.0f;
            }
        }
        if (currentState && !transitioning) {
            currentState->Update(deltaTime, countryPtr);
            currentState->LateUpdate();
        }
    }

    void Draw() {
        if (currentState) currentState->Draw();

        if (transitioning) {
            float progress = transitionTimer / TRANSITION_DURATION;

            if (progress <= MID_TRANSITION_THRESHOLD) {
                // Faza 1: Zasłanianie od góry
                float normalizedPhase = PHASE_OFFSET + (progress * ANIMATION_SPEED_SCALE);
                float curtainY = normalizedPhase * Config::SCREEN_HEIGHT;
                DrawRectangle(0, 0, Config::SCREEN_WIDTH, (int)curtainY, BLACK);
                DrawRectangleGradientV(0, (int)curtainY, Config::SCREEN_WIDTH, (int)GRADIENT_HEIGHT, BLACK, BLANK);
            } else {
                // Faza 2: Odsłanianie (górna krawędź "kurtyny" zjeżdża w dół)
                float normalizedPhase = (progress - MID_TRANSITION_THRESHOLD) * ANIMATION_SPEED_SCALE;
                float curtainY = normalizedPhase * Config::SCREEN_HEIGHT;
                DrawRectangle(0, (int)curtainY, Config::SCREEN_WIDTH, (int)(Config::SCREEN_HEIGHT - curtainY), BLACK);
                DrawRectangleGradientV(0, (int)(curtainY - GRADIENT_HEIGHT), Config::SCREEN_WIDTH, (int)GRADIENT_HEIGHT, BLANK, BLACK);
            }
        }
    }

	// Nie zezwalaj na kopiowanie
    StateManager(const StateManager&) = delete;
    void operator=(const StateManager&) = delete;
};

#endif
