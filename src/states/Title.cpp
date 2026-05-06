#include <src/backend/GameState.h>
#include <src/Constants.h>

class Title : public GameState {
	public:

	Image umk_img;
	Texture2D umk_logo;

	void Init() override {
		umk_img = LoadImage("images/title/umk.png");
		ImageResize(&umk_img, umk_img.width / 5, umk_img.height / 5);

		umk_logo = LoadTextureFromImage(umk_img);
	}

	void Update(float dt) override {

	}

	void Draw() override {
		DrawTexture(umk_logo, 5, Config::SCREEN_HEIGHT - umk_logo.height - 5, WHITE);
	}
};