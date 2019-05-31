#pragma once

#include <SFML/Graphics.hpp>

#ifdef _IS_WINDOWS
#  include <functional>
#endif

typedef std::function<void(const sf::Event &)>              event_callback_t;
typedef std::function<void(sf::RenderWindow &)>             draw_callback_t;
typedef std::function<void(const int, const std::string &)> menu_callback_t;

class NUI_MenuBar;

struct NUI_Window {
    virtual void set_title(const std::string &s) = 0;
    virtual bool add_menubar(NUI_MenuBar *n) = 0;

    virtual void set_position(int x, int y) = 0;
    virtual void set_dimensions(int w, int h) = 0;

    virtual int run_loop() = 0;

    virtual void set_on_event_callback(event_callback_t) = 0;
    virtual void set_on_draw_callback(draw_callback_t) = 0;
    virtual void set_on_menu_callback(menu_callback_t) = 0;

    virtual void message_box(std::string &, std::string &) = 0;
;
protected:
    NUI_Window() {
        on_event_callback = on_event_default_callback;
        on_draw_callback = on_draw_default_callback;
        on_menu_callback = on_menu_default_callback;
    }

    virtual ~NUI_Window() {}

    event_callback_t on_event_callback;
    draw_callback_t  on_draw_callback;
    menu_callback_t  on_menu_callback;

    event_callback_t on_event_default_callback = [](const sf::Event &) -> void {
        return;
    };

    draw_callback_t on_draw_default_callback = [](sf::RenderWindow &w) -> void {
        w.clear();
        w.display();
    };

    menu_callback_t on_menu_default_callback = [](const int, const std::string &) -> void {
        return;
    };
};