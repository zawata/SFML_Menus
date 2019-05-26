#pragma once

#include "window.hpp"
#include <gtkmm/applicationwindow.h>

#include "gtk/sfml_widget.hpp"

struct NUI_Window_gtk : public NUI_Window, public Gtk::Application {
    // NUI_Window
    NUI_Window_gtk(const std::string &title);
    NUI_Window_gtk(const std::string &title, int w, int h);

    bool add_menubar(NUI_MenuBar *n) override;

    void set_title(const std::string &s) override;

    void set_position(int x, int y) override;
    void set_dimensions(int w, int h) override;

    void set_on_event_callback(event_callback_t) override;
    void set_on_draw_callback(draw_callback_t) override;
    void set_on_menu_callback(menu_callback_t) override;

    int run_loop() override;

    //GTK::ApplicationWindow
protected:
    // Override default signal handlers:
    void on_activate() override;
    void on_startup() override;
    void on_window_hide(Gtk::ApplicationWindow* window);

    void set_menubar_gtk();

private:
    bool initialized = false;
    NUI_MenuBar *menu;
    Gtk::ApplicationWindow *main_window;
    SFMLWidget *sfml_widget;
};