#include <src/backend/GameState.h>
#include <src/backend/StateManager.h>
#include <src/Constants.h>
#include <src/backend/UIHelpers.h>
#include <cmath>
#include <cstdlib>

// Dekalarcje dla stanów użytych przez Update
class Visualization;
class Credits;

class Title : public GameState {
	public:

	Texture2D marble_bg;
	Texture2D main_logo;
	Texture2D umk_logo;
	Font customFont;

	float introTimer = 0.0f;
	const float fadeDuration = 2.0f; // Sekundy przejścia
	int selectedItem = 0;
	int lastSelectedItem = 0;

	// Prostokąty przycisków
	Rectangle btnVis = { 0 };
	Rectangle btnCredits = { 0 };
	Rectangle btnExit = { 0 };
	Rectangle btnUmk = { 0 };

	void Init() override {
		marble_bg = LoadTexture(UI::AssetPath("images/ui/marble.png"));

		Image umk_img = LoadImage(UI::AssetPath("images/title/umk.png"));
		ImageResize(&umk_img, umk_img.width / 5, umk_img.height / 5);
		umk_logo = LoadTextureFromImage(umk_img);
		UnloadImage(umk_img);

		Image main_img = LoadImage(UI::AssetPath("images/title/logo.png"));
		ImageResize(&main_img, main_img.width * 1.5, main_img.height * 1.5);
		main_logo = LoadTextureFromImage(main_img);
		UnloadImage(main_img);

		customFont = UI::LoadStandardFont(40);

		float centerX = Config::SCREEN_WIDTH / 2.0f;
		btnVis = { centerX - 100, Config::SCREEN_HEIGHT / 2.0f + 50, 200, 40 };
		btnCredits = { centerX - 100, Config::SCREEN_HEIGHT / 2.0f + 110, 200, 40 };
		btnExit = { centerX - 100, Config::SCREEN_HEIGHT / 2.0f + 170, 200, 40 };
		btnUmk = { 5.0f, (float)Config::SCREEN_HEIGHT - umk_logo.height - 5, (float)umk_logo.width, (float)umk_logo.height };
	}

	void Update(float dt) override;

	void Draw() override {
		UI::DrawTiledBackground(marble_bg);

		float alpha = fminf(introTimer / fadeDuration, 1.0f);
		float posY = (Config::SCREEN_HEIGHT / 4.0f) * alpha; // Idź z góry

		float swayX = sinf(GetTime() * 2.0f) * 15.0f;
		float centerX = (Config::SCREEN_WIDTH / 2.0f) - (main_logo.width / 2.0f);

		DrawTexture(main_logo, centerX + swayX, posY - 50, Fade(WHITE, alpha));

		UI::DrawMenuButton(btnVis, "WIZUALIZACJA", customFont);
		UI::DrawMenuButton(btnCredits, "TWÓRCY", customFont);
		UI::DrawMenuButton(btnExit, "WYJŚCIE", customFont);

		// Rysowanie ramki wyboru
		Rectangle r = (selectedItem == 0) ? btnVis : (selectedItem == 1 ? btnCredits : btnExit);
		DrawRectangleLinesEx({ r.x - 5, r.y - 5, r.width + 10, r.height + 10 }, 2, GOLD);

		DrawTexture(umk_logo, 5, Config::SCREEN_HEIGHT - umk_logo.height - 5, WHITE);
	}

	~Title() {
		UnloadTexture(marble_bg);
		UnloadTexture(main_logo);
		UnloadTexture(umk_logo);
		UnloadFont(customFont);
	}
};

// Definicje innych ekranów po deklaracji głównego menu
#include <src/states/Visualization.cpp>
#include <src/states/Credits.cpp>
#include <src/backend/UIHelpers.h>

// Definicja logiki aktualizacji
void Title::Update(float dt) {
	introTimer += dt;

	// Sterowanie klawiaturą - nawigacja
	if (IsKeyPressed(KEY_UP)) selectedItem = (selectedItem + 2) % 3;
	if (IsKeyPressed(KEY_DOWN)) selectedItem = (selectedItem + 1) % 3;

	// Obsługa myszy
	Vector2 mouse = GetMousePosition();
	if (CheckCollisionPointRec(mouse, btnVis)) selectedItem = 0;
	else if (CheckCollisionPointRec(mouse, btnCredits)) selectedItem = 1;
	else if (CheckCollisionPointRec(mouse, btnExit)) selectedItem = 2;

	// Zmiana kursora na rączkę przy najechaniu na logo UMK lub przyciski menu
	if (CheckCollisionPointRec(mouse, btnUmk) || CheckCollisionPointRec(mouse, btnVis) ||
		CheckCollisionPointRec(mouse, btnCredits) || CheckCollisionPointRec(mouse, btnExit)) {
		SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
	} else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

	// Dźwięk najechania (tylko przy zmianie)
	if (selectedItem != lastSelectedItem) {
		UI::PlayHoverSound();
		lastSelectedItem = selectedItem;
	}

	// Potwierdzenie wyboru klawiaturą (ENTER)
	if (IsKeyPressed(KEY_ENTER)) {
		UI::PlaySelectSound();
		if (selectedItem == 0) {
			StateManager::Instance().ChangeState(new Visualization());
		}
		else if (selectedItem == 1) {
			StateManager::Instance().ChangeState(new Credits());
		}
		else if (selectedItem == 2) {
			exit(0);
		}
	}
	// Potwierdzenie wyboru myszą (LEFT_BUTTON)
	else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(mouse, btnUmk)) {
			UI::PlaySelectSound();
			OpenURL("https://www.umk.pl/");
		} else if (CheckCollisionPointRec(mouse, btnVis)) {
			UI::PlaySelectSound();
			StateManager::Instance().ChangeState(new Visualization());
		} else if (CheckCollisionPointRec(mouse, btnCredits)) {
			UI::PlaySelectSound();
			StateManager::Instance().ChangeState(new Credits());
		} else if (CheckCollisionPointRec(mouse, btnExit)) {
			UI::PlaySelectSound();
			exit(0);
		}
	}

	if (IsKeyPressed(KEY_ESCAPE)) exit(0);
}