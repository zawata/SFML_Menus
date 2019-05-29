#pragma once

#define NOMINMAX 1                      // Turn off min and max macros, they conflict std::min/max
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "window_base.hpp"
#include "menubar.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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

    int run_loop() override;
private:
    LRESULT CALLBACK on_win32_event(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static HWND InitInstance(std::wstring);
    static HWND InitSFML(sf::RenderWindow &, HWND);
    static ATOM MyRegisterClass();
    static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

    HWND main_hwnd,
         sfml_hwnd;
    sf::RenderWindow sfml_window;
    bool menubar_locked = false;
};
