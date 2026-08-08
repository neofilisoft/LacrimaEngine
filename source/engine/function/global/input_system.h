#pragma once

#include "engine/core/event/event_system.h"
#include <glm/glm.hpp>
#include <array>

namespace Lacrima
{
    class InputSystem
    {
    public:
        InputSystem();
        ~InputSystem();

        void init();
        void destroy();

        void tick();

        bool isKeyDown(int key) const;
        bool isKeyJustPressed(int key) const;
        bool isMouseButtonDown(int button) const;
        glm::vec2 getMousePosition() const { return m_mouse_position; }
        glm::vec2 getMouseScroll() const { return m_mouse_scroll; }

    private:
        void onKey(int key, int action);
        void onMouseButton(int button, int action);
        void onCursorPos(double xpos, double ypos);
        void onScroll(double xoffset, double yoffset);

        std::array<bool, 512> m_keys_down{};
        std::array<bool, 512> m_keys_just_pressed{};
        std::array<bool, 8> m_mouse_buttons_down{};
        glm::vec2 m_mouse_position{0.0f, 0.0f};
        glm::vec2 m_mouse_scroll{0.0f, 0.0f};

        void* m_key_listener = nullptr;
        void* m_mouse_button_listener = nullptr;
        void* m_cursor_pos_listener = nullptr;
        void* m_scroll_listener = nullptr;
    };
}
