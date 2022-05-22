#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui_stdlib.h"
#include "engine/GameCore.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

enum class ImGuiTheme: int { dark, light, classic };

class GameLauncher
{
public:
    // Dear ImGui theme.
    ImGuiTheme GuiTheme = ImGuiTheme::dark;

    GameLauncher(GameCore* instance);
    ~GameLauncher() {}

    void Launch();
    GameLauncher& Init();
    void SetCursorMode(int mode);
protected:
    inline static GameCore* game_instance = nullptr;
    GLFWwindow* window = nullptr;
    bool bInitialized = false;

    void init_glfw();
    void init_glad();
    void init_opengl();
    void init_imgui();
    void end();

    friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};