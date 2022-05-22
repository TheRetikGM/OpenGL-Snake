#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "engine/GLFWInputEnums.h"

/*
*  Simple keyboard interface wrapper for checking key presses.
*/
class InputInterface
{
public:
    InputInterface(bool* pKeys, bool* pKeysProcessed, bool* pMouseButtons, bool* pMouseButtonsPressed) 
        : pKeys(pKeys)
        , pKeysProcessed(pKeysProcessed)
        , pMouseButtons(pMouseButtons)
        , pMouseButtonsPressed(pMouseButtonsPressed)
        , vMousePosition(glm::vec2(0.0f))
    {}

    // Checks ifs key was pressed (will return true only once until it is released).
    inline bool Pressed(Key key)
    {
        if (pKeys[int(key)] && !pKeysProcessed[int(key)])
        {
            pKeysProcessed[int(key)] = true;
            return true;
        }
        return false;
    }
    // Checks if key is being held.
    inline bool Held(Key key)
    {
        return pKeys[int(key)];
    }

    inline bool MousePressed(MouseButton key)
    {
        if (pMouseButtons[int(key)] && !pMouseButtonsPressed[int(key)])
        {
            pMouseButtonsPressed[int(key)] = true;
            return true;
        }
        return false;
    }
    inline bool MouseHeld(MouseButton key)
    {
        return pMouseButtons[int(key)];
    }
    const glm::vec2& GetMousePos() { return vMousePosition; }

protected:
    bool* pKeys;
    bool* pKeysProcessed;
    bool* pMouseButtons;
    bool* pMouseButtonsPressed;
    glm::vec2 vMousePosition;

    friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};