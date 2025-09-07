#pragma once
#include "Luhame/Core/window.h"
#include<GLFW/glfw3.h>

namespace Luhame {
    class windows_window : public window
    {
    public:
        windows_window(const window_props& props);
        virtual ~windows_window();
        inline void set_event_callback(const event_callback_fn& callback) override {
            m_window_data.m_event_callback_fn = callback;
        }
        inline uint32_t get_width() const override { return m_window_data.m_width; }
        inline uint32_t get_height() const override { return m_window_data.m_height; }

        void on_update() override;

    private:
        virtual void init(const window_props& props);
        virtual void shut_down();
        void set_VSync(bool enable);
    private:

        struct window_data {
            std::string m_title;
            event_callback_fn m_event_callback_fn;
            uint32_t m_width, m_height;
            bool VSync;
        };

        GLFWwindow* m_window;

        window_data m_window_data;
    };
}

