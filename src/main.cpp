#include <engine/Engine.h>
#include <vector>
#include "Map.hpp"

unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 600;

class Game : public GameCore
{
	Map* map = nullptr;
	bool bShowOptions = false;
	glm::ivec2 vMapSize = { 30, 30 };

public:
	Game(unsigned int width, unsigned int height) : GameCore(width, height) {}
	~Game() {}

	void Init()
	{
		ResourceManager::LoadTexture(ASSETS_DIR "awesomeface.png", true, "face");
		map = new Map(vMapSize);
		map->Init();
	}
	void Delete()
	{
		delete map;
	}
	void ProcessInput()
	{
		if (Input->Pressed(Key::ESCAPE))
			bShowOptions = !bShowOptions;
		
		if (!bShowOptions)
			map->ProcessInput(Input);
	}
	void Update(float dt)
	{
		if (!bShowOptions)
			map->Update(dt);
	}
	void Render()
	{
		map->Render(basic_renderer.get(), sprite_renderer.get());

		if (map->bGameOver)
		{
			ImGui::Begin("Game Over!", NULL, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("You lost!");
			ImGui::LabelText("Score", "%i", map->GetScore());
			float tim = map->GetTime();
			int hours = int(tim) / 3600;
			int minutes = (int(tim) / 60) % 60;
			float seconds = std::fmod(tim, 60.0f);
			ImGui::LabelText("Time", "%02i:%02i:%05.3f", hours, minutes, seconds);
			if (ImGui::Button("Exit"))
				Exit();
			ImGui::SameLine();
			if (ImGui::Button("Restart"))
				RestartGame();
			ImGui::End();
		}
		if (bShowOptions)
		{
			ImGui::Begin("Pause menu");
			ImGui::Text("Configuration");
			if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorEdit3("Background color", (float*)&BackgroundColor, ImGuiColorEditFlags_PickerHueWheel);
				ImGui::ColorEdit3("Map color", (float*)&map->MapConf.vMapColor, ImGuiColorEditFlags_PickerHueWheel);
				ImGui::ColorEdit3("Snake color", (float*)&map->MapConf.vSnakeColor, ImGuiColorEditFlags_PickerHueWheel);
				ImGui::ColorEdit3("Food color", (float*)&map->MapConf.vFoodColor, ImGuiColorEditFlags_PickerHueWheel);
				ImGui::ColorEdit3("Blink color", (float*)&map->MapConf.vBlinkColor, ImGuiColorEditFlags_PickerHueWheel);
			}
			if (ImGui::CollapsingHeader("Size and speed", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::DragInt2("Tile spacing", (int*)&map->MapConf.vSpacing, 1, 0, 100);
				ImGui::DragInt2("Map offset", (int*)&map->MapConf.vMapOffset, 5, 0, 500);
				ImGui::DragInt2("Part size", (int*)&map->MapConf.vPartSize, 1, 0, 100);
				static int snake_move_speed = map->MapConf.nSnakeMoveSpeed;
				ImGui::DragInt("Snake movement speed", &snake_move_speed, 1, 1, 1000);
				map->SetSnakeMovementSpeed(snake_move_speed);
				ImGui::InputInt2("Map size (restart needed)", (int*)&vMapSize);
			}
			if (ImGui::CollapsingHeader("Misc", ImGuiTreeNodeFlags_DefaultOpen))
			{
				int nSnakeGrow = map->MapConf.GetSnakeGrow();
				ImGui::InputInt("Spawned food (restart needed)", &map->MapConf.nInitSpawnFood);
				ImGui::InputInt("Snake eat grow", &nSnakeGrow);
				ImGui::Checkbox("Buffer movement", &map->MapConf.bBufferMovementCommands);

				if (nSnakeGrow != map->MapConf.GetSnakeGrow())
					map->MapConf.SetSnakeGrow(nSnakeGrow);
			}
			ImGui::Text("Options");
			if (ImGui::Button("Exit"))
				Exit();
			ImGui::SameLine();
			if (ImGui::Button("Restart"))
				RestartGame();
			ImGui::SameLine();
			if (ImGui::Button("Close"))
				bShowOptions = false;
			ImGui::End();
		}
	}

	void RestartGame()
	{
		Configuration bck = map->MapConf;
		delete map;
		map = new Map(vMapSize);
		map->MapConf = bck;
		map->Init();
	}
};

int main()
{
	GameCore* game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);
	GameLauncher launcher(game);
	launcher.GuiTheme = ImGuiTheme::dark;
	launcher.Init().Launch();

	delete game;
	return 0;
}
