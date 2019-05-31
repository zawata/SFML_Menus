#pragma once

#define NOMINMAX 1                      // Turn off min and max macros, they conflict std::min/max
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "window_base.hpp"
#include "menubar.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>

struct NUI_Window_win32 : public NUI_Window {
    NUI_Window_win32(const std::string &s);
    NUI_Window_win32(const std::string &s, int w, int h);

    void set_title(const std::string &s) override;
    bool add_menubar(NUI_MenuBar *n) override;

    void set_position(int x, int y) override;
    void set_dimensions(int w, int h) override;

    virtual void set_on_event_callback(event_callback_t);
    virtual void set_on_draw_callback(draw_callback_t);
    virtual void set_on_menu_callback(menu_callback_t);

    void message_box(std::string &, std::string &) override;

    int run_loop() override;
private:
    LRESULT CALLBACK on_win32_event(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static HWND initInstance(std::wstring);
    static HWND initSFML(sf::RenderWindow &, HWND);
    static ATOM registerClass();
    static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

    HWND main_hwnd,
         sfml_hwnd;
    sf::RenderWindow sfml_window;
    bool menubar_locked = false;

    std::unordered_map<int, std::pair<NUI_MenuItem *, std::function<void (NUI_MenuItem *)>>> callback_map;
};
