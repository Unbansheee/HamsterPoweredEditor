#include "Input.h"

#include <array>

#include "Window.h"

bool Input::IsKeyDown(Keyboard::Key key)
{
    return m_KeyboardStates[key];
}

bool Input::WasKeyPressed(Keyboard::Key key)
{
    return m_KeyboardStates[key] && !m_PrevKeyboardStates[key];
}

bool Input::WasKeyReleased(Keyboard::Key key)
{
    return !m_KeyboardStates[key] && m_PrevKeyboardStates[key];
}

bool Input::IsMouseButtonDown(Mouse::Button button)
{
    return m_MouseButtonStates[button];
}

bool Input::WasMouseButtonPressed(Mouse::Button button)
{
    return m_MouseButtonStates[button] && !m_PrevMouseButtonStates[button];
}

bool Input::WasMouseButtonReleased(Mouse::Button button)
{
    return !m_MouseButtonStates[button] && m_PrevMouseButtonStates[button];
}

glm::vec2 Input::GetMousePosition()
{
    return m_MousePosition;
}

glm::vec2 Input::GetLastMousePosition()
{
    return m_PreviousMousePosition;
}

glm::vec2 Input::GetMouseDelta()
{
    return m_MouseDelta;
}

float Input::GetMouseWheelDelta()
{
    return m_MouseScrollDelta;
}

void Input::Reset()
{
    m_PrevMouseButtonStates = m_MouseButtonStates;
    m_PrevKeyboardStates = m_KeyboardStates;
    
    
    m_MouseScrollDelta = 0;
    m_PreviousMousePosition = m_MousePosition;
    m_MousePosition = Context->GetMousePosition();
    m_MouseDelta = m_MousePosition - m_PreviousMousePosition;
    
    
}
