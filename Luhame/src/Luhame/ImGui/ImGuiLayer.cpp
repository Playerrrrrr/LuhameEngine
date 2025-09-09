#include "pch.h"
#include "ImGuiLayer.h"
#include"imgui/imgui.h"
#include"Luhame/Core/Application.h"
#include"GLFW/glfw3.h"
#include"imgui/backends/imgui_impl_opengl3.h"
#include"imgui/backends/imgui_impl_glfw.h"
#include"Luhame/Core/Input.h"
namespace Luhame {




    imgui_layer::imgui_layer()
        : layer("ImGUI Layer")
    {

    }

    imgui_layer::imgui_layer(const std::string& name)
        : layer(name)
    {

    }

    imgui_layer::~imgui_layer()
    {

    }

    void imgui_layer::on_attach()
    {

    }

    void imgui_layer::on_detach()
    {
    }

    void imgui_layer::on_update()
    {


    }

    void imgui_layer::on_event(event& event)
    {
        event_dispatcher dispatcher(event);
        dispatcher.dispatch<mouse_button_pressed_event>(LH_BIND_EVENT_FN(imgui_layer::on_mouse_button_pressed_event));
        dispatcher.dispatch<mouse_button_released_event>(LH_BIND_EVENT_FN(imgui_layer::on_mouse_button_released_event));
        dispatcher.dispatch<mouse_moved_event>(LH_BIND_EVENT_FN(imgui_layer::on_mouse_moved_event));
        dispatcher.dispatch<mouse_scrolled_event>(LH_BIND_EVENT_FN(imgui_layer::on_mouse_scrolled_event));
        dispatcher.dispatch<key_pressed_event>(LH_BIND_EVENT_FN(imgui_layer::on_key_pressed_event));
        dispatcher.dispatch<key_released_event>(LH_BIND_EVENT_FN(imgui_layer::on_key_released_event));
        dispatcher.dispatch<window_resize_event>(LH_BIND_EVENT_FN(imgui_layer::on_window_resize_event));
        dispatcher.dispatch<key_typed_event>(LH_BIND_EVENT_FN(imgui_layer::on_key_typed_event));

    }
    bool imgui_layer::on_mouse_button_pressed_event(mouse_button_pressed_event& e)
    {
        LH_CORE_INFO("{}", e);

        return false;
    }

    bool imgui_layer::on_mouse_button_released_event(mouse_button_released_event& e)
    {
        LH_CORE_INFO("{}", e);

        return false;
    }

    bool imgui_layer::on_mouse_moved_event(mouse_moved_event& e)
    {
        LH_CORE_INFO("{}", e);

        return false;
    }

    bool imgui_layer::on_mouse_scrolled_event(mouse_scrolled_event& e)
    {
        LH_CORE_INFO("{}", e);

        return false;
    }

    bool imgui_layer::on_key_pressed_event(key_pressed_event& e)
    {
        LH_CORE_INFO("{}", e);
        return false;
    }
    bool imgui_layer::on_key_typed_event(key_typed_event& e)
    {
        LH_CORE_INFO("{}", e);
        return false;
    }
    bool imgui_layer::on_key_released_event(key_released_event& e)
    {
        LH_CORE_INFO("{}", e);

        return false;
    }

    bool imgui_layer::on_window_resize_event(window_resize_event& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.get_width(), e.get_height());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.get_width(), e.get_height());
        return false;
    }

}