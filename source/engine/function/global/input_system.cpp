#include "input_system.h"
#include "engine/function/global/engine_context.h"
#include "engine/core/base/macro.h"

#ifndef GLFW_PRESS
#define GLFW_PRESS 1
#endif
#ifndef GLFW_RELEASE
#define GLFW_RELEASE 0
#endif

namespace Lacrima
{
    InputSystem::InputSystem()
    {
    }

    InputSystem::~InputSystem()
    {
        destroy();
    }

    void InputSystem::init()
    {
        m_key_listener = g_engine.eventSystem()->addListener(EEventType::WindowKey, [this](const EventPointer& e) {
            auto key_event = std::static_pointer_cast<WindowKeyEvent>(e);
            onKey(key_event->key, key_event->action);
        });

        m_mouse_button_listener = g_engine.eventSystem()->addListener(EEventType::WindowMouseButton, [this](const EventPointer& e) {
            auto mouse_event = std::static_pointer_cast<WindowMouseButtonEvent>(e);
            onMouseButton(mouse_event->button, mouse_event->action);
        });

        m_cursor_pos_listener = g_engine.eventSystem()->addListener(EEventType::WindowCursorPos, [this](const EventPointer& e) {
            auto cursor_event = std::static_pointer_cast<WindowCursorPosEvent>(e);
            onCursorPos(cursor_event->xpos, cursor_event->ypos);
        });

        m_scroll_listener = g_engine.eventSystem()->addListener(EEventType::WindowScroll, [this](const EventPointer& e) {
            auto scroll_event = std::static_pointer_cast<WindowScrollEvent>(e);
            onScroll(scroll_event->xoffset, scroll_event->yoffset);
        });

        LOG_INFO("InputSystem initialized successfully");
    }

    void InputSystem::destroy()
    {
        if (g_engine.eventSystem())
        {
            if (m_key_listener) g_engine.eventSystem()->removeListener(m_key_listener);
            if (m_mouse_button_listener) g_engine.eventSystem()->removeListener(m_mouse_button_listener);
            if (m_cursor_pos_listener) g_engine.eventSystem()->removeListener(m_cursor_pos_listener);
            if (m_scroll_listener) g_engine.eventSystem()->removeListener(m_scroll_listener);
        }
        m_key_listener = nullptr;
        m_mouse_button_listener = nullptr;
        m_cursor_pos_listener = nullptr;
        m_scroll_listener = nullptr;
    }

    void InputSystem::tick()
    {
        m_keys_just_pressed.fill(false);
        m_mouse_scroll = glm::vec2(0.0f, 0.0f);
    }

    bool InputSystem::isKeyDown(int key) const
    {
        if (key >= 0 && key < 512)
        {
            return m_keys_down[key];
        }
        return false;
    }

    bool InputSystem::isKeyJustPressed(int key) const
    {
        if (key >= 0 && key < 512)
        {
            return m_keys_just_pressed[key];
        }
        return false;
    }

    bool InputSystem::isMouseButtonDown(int button) const
    {
        if (button >= 0 && button < 8)
        {
            return m_mouse_buttons_down[button];
        }
        return false;
    }

    void InputSystem::onKey(int key, int action)
    {
        if (key >= 0 && key < 512)
        {
            if (action == GLFW_PRESS)
            {
                m_keys_down[key] = true;
                m_keys_just_pressed[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                m_keys_down[key] = false;
            }
        }
    }

    void InputSystem::onMouseButton(int button, int action)
    {
        if (button >= 0 && button < 8)
        {
            if (action == GLFW_PRESS)
            {
                m_mouse_buttons_down[button] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                m_mouse_buttons_down[button] = false;
            }
        }
    }

    void InputSystem::onCursorPos(double xpos, double ypos)
    {
        m_mouse_position.x = static_cast<float>(xpos);
        m_mouse_position.y = static_cast<float>(ypos);
    }

    void InputSystem::onScroll(double xoffset, double yoffset)
    {
        m_mouse_scroll.x = static_cast<float>(xoffset);
        m_mouse_scroll.y = static_cast<float>(yoffset);
    }
}
