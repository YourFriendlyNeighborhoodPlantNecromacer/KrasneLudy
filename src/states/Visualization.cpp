#include <src/backend/GameState.h>
#include <src/backend/StateManager.h>
#include <src/Constants.h>
#include <src/backend/UIHelpers.h>
//#include <src/DataStructures/DataStructures.h> //Na później

class Visualization : public GameState {
	public:

	Texture2D marble_bg;
	Rectangle viewportArea;
	Camera2D camera;

	void Init() override {
		marble_bg = LoadTexture("images/ui/marble.png");

		float padding = 40.0f;
		float bottomSpace = 140.0f; // Miejsce na przyciski i rezultaty i takie tam

		viewportArea = {
			padding,
			padding,
			(float)Config::SCREEN_WIDTH - (padding * 2),
			(float)Config::SCREEN_HEIGHT - padding - bottomSpace
		};

		// Inicjalizacja kamery
		camera.target = { 0, 0 };
		camera.offset = { viewportArea.x + viewportArea.width / 2.0f, viewportArea.y + viewportArea.height / 2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 1.0f;
	}

	void Update(float dt) override {
		if (IsKeyPressed(KEY_ESCAPE)) {
			StateManager::Instance().ChangeState(new Title());
		}

		// Logika kamery
		if (CheckCollisionPointRec(GetMousePosition(), viewportArea)) {
			float wheel = GetMouseWheelMove();
			if (wheel != 0) {
				Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
				camera.offset = GetMousePosition();
				camera.target = mouseWorldPos;

				const float zoomSpeed = 0.125f;
				camera.zoom += wheel * zoomSpeed;
				if (camera.zoom < 0.1f) camera.zoom = 0.1f;
				if (camera.zoom > 10.0f) camera.zoom = 10.0f;
			}

			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
				Vector2 delta = GetMouseDelta();
				camera.target.x -= delta.x / camera.zoom;
				camera.target.y -= delta.y / camera.zoom;
			}
		}
	}

	void Draw() override {
		UI::DrawTiledBackground(marble_bg);

		// Pokolorowanie części dla odróżnienia
		DrawRectangleRec(viewportArea, Fade(BLACK, 0.15f));

		// Przycinanie do określonego kształtu
		BeginScissorMode((int)viewportArea.x, (int)viewportArea.y, (int)viewportArea.width, (int)viewportArea.height);
			BeginMode2D(camera);
				for (int i = -2000; i <= 2000; i += 100) {
					DrawLine(i, -2000, i, 2000, Fade(RAYWHITE, 0.2f));
					DrawLine(-2000, i, 2000, i, Fade(RAYWHITE, 0.2f));
				}

				DrawCircle(0, 0, 10, RED);

				// TODO: Rysować grafy.

			EndMode2D();
		EndScissorMode();

		DrawRectangleLinesEx(viewportArea, 3, RAYWHITE);
	}

	~Visualization() {
		UnloadTexture(marble_bg);
	}
};