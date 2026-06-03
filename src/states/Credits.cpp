#include <src/backend/GameState.h>
#include <src/backend/StateManager.h>
#include <src/Constants.h>
#include <src/backend/UIHelpers.h>

class Credits : public GameState {
	public:

	Texture2D marble_bg;
	Rectangle btnBack = { 0 };
	Font customFont;

	void Init() override {
		marble_bg = LoadTexture("images/ui/marble.png");

		customFont = UI::LoadStandardFont(40);

		// Przycisk zamknięcia
		btnBack = { 20, 20, 40, 40 };
	}

	void Update(float dt) override {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			if (CheckCollisionPointRec(GetMousePosition(), btnBack)) {
				StateManager::Instance().ChangeState(new Title());
			}
		}

		// Wyjść też można przez ESC
		if (IsKeyPressed(KEY_ESCAPE)) {
			StateManager::Instance().ChangeState(new Title());
		}
	}

	void Draw() override {
		// Rysowanie tła
		UI::DrawTiledBackground(marble_bg);

		// Rysowanie imion
		const char* titleText = "TWÓRCY ROZWIĄZANIA PROJEKTU:";
		const char* names[] = {
			"Szymon Zakierski",
			"Antoni Narożny",
			"Dawid",
			"Franciszek"
		};

		Vector2 titleSize = MeasureTextEx(customFont, titleText, 40, 2);
		DrawTextEx(customFont, titleText, { (Config::SCREEN_WIDTH - titleSize.x) / 2, 100 }, 40, 2, DARKGRAY);

		for (int i = 0; i < 4; i++) {
			int posY = 220 + (i * 60);
			Vector2 nameSize = MeasureTextEx(customFont, names[i], 30, 2);
			DrawTextEx(customFont, names[i], { (Config::SCREEN_WIDTH - nameSize.x) / 2, (float)posY }, 30, 2, BLACK);
		}

		// Rysowanie przycisku wyjścia
		bool hover = CheckCollisionPointRec(GetMousePosition(), btnBack);
		DrawRectangleRec(btnBack, hover ? RED : MAROON);
		DrawRectangleLinesEx(btnBack, 2, BLACK);
		DrawText("X", btnBack.x + 12, btnBack.y + 8, 25, WHITE);
	}

	~Credits() {
		UnloadTexture(marble_bg);
		UnloadFont(customFont);
	}
};