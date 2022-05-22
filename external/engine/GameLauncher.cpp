#include <glad/glad.h>
#include "engine/GameLauncher.h"
#include <GLFW/glfw3.h>
#include "engine/DebugColors.h"
#include "engine/Logger.hpp"
#include "engine/Core.h"

ImGuiIO* imgui_io = nullptr;

inline float get_time_from_start()
{
    return float(glfwGetTime());
}

GameLauncher::GameLauncher(GameCore* instance)
{
    game_instance = instance;
}

GameLauncher& GameLauncher::Init()
{
    init_glfw();
    init_glad();
    init_imgui();
    init_opengl();
    game_instance->GetTimeFromStart = &get_time_from_start;
    bInitialized = true;
    return *this;
}
void GameLauncher::Launch()
{
    if (!bInitialized)
    {
        LOG_C("Launcher not initialized! Maybe call GameLauncher::Init()");
        return;
    }

    try
    {
        game_instance->InitEngine();
        game_instance->Init();

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window) && game_instance->Run)
        {
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            // Poll events like key presses, mouse event, ...
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            game_instance->ProcessInput();
            game_instance->Update(deltaTime);

            // Set window title
            glfwSetWindowTitle(window, game_instance->WindowTitle.c_str());

            // Clear default framebuffer and render game scene.
            glClearColor(game_instance->BackgroundColor.r, game_instance->BackgroundColor.g, game_instance->BackgroundColor.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            game_instance->Render();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change he current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere)
            if (imgui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            // Swap back and front buffers.
            glfwSwapBuffers(window);
        }
        if (!game_instance->Run)
            glfwSetWindowShouldClose(window, true);
    }
    catch (const std::exception& e)
    {
        LOG_E(e.what());
    }
    end();
}
void GameLauncher::SetCursorMode(int mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}
void GameLauncher::init_glfw()
{
    /* Initialize glfw and create window */
    if (!glfwInit())
    {
        std::cerr << DC_ERROR " Failed to initialize GLFW." << std::endl;
        return;
    }
    
    // Set window properties.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and assign callbacks.
    window = glfwCreateWindow(game_instance->Width, game_instance->Height, game_instance->WindowTitle.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSwapInterval(1);
}
void GameLauncher::init_glad()
{
    /* Load GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("GameLauncher::init_glad(): Failed to initialize GLAD.");
    }
}
void GameLauncher::init_opengl()
{
    // OpenGL configuration
    glViewport(0, 0, game_instance->Width, game_instance->Height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST | GL_CULL_FACE);
}
void GameLauncher::init_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    imgui_io = &io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable docking.
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-View / Platform Windows.

    if (GuiTheme == ImGuiTheme::dark)
        ImGui::StyleColorsDark();
    else if (GuiTheme == ImGuiTheme::classic)
        ImGui::StyleColorsClassic();
    else
        ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
void GameLauncher::end()
{
    game_instance->Delete();
    game_instance->DeleteEngine();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	GameLauncher::game_instance->Width = width;
	GameLauncher::game_instance->Height = height;
	GameLauncher::game_instance->OnResize();
    GameLauncher::game_instance->_onResize();
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{		
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			GameLauncher::game_instance->Keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			GameLauncher::game_instance->Keys[key] = false;
			GameLauncher::game_instance->KeysProcessed[key] = false;
		}
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	GameLauncher::game_instance->ProcessMouse(float(xpos), float(ypos));
    GameLauncher::game_instance->Input->vMousePosition = glm::vec2(float(xpos), float(ypos));
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	GameLauncher::game_instance->ProcessScroll((float)yoffset);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= 0 && button < 8)
    {
        if (action == GLFW_PRESS)
            GameLauncher::game_instance->MouseButtons[button] = true;
        else if (action == GLFW_RELEASE)
        {
            GameLauncher::game_instance->MouseButtons[button] = false;
            GameLauncher::game_instance->MouseButtonsPressed[button] = false;
        }
    }
}