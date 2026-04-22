#include <src/backend/GameState.h>

#define FOV_ORTHOGRAPHIC 10.0f

class Visualization : public GameState {
	public:

    Camera camera;

	void Init() override {
		camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, FOV_ORTHOGRAPHIC, CAMERA_ORTHOGRAPHIC };
	}

	void Update(float dt) override {

	}

	void Draw() override {
		BeginMode3D(camera);

			DrawCube((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, RED);

			DrawGrid(10, 1.0f);

		EndMode3D();
	}
};