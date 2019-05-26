#include "window.hpp"
#include "gtk/window_impl-gtk.hpp"

NUI_Window *createWindow(const std::string &s) {
    return new NUI_Window_gtk(s);
}

NUI_Window *createWindow(const std::string &s, int x, int y) {
    NUI_Window *n = new NUI_Window_gtk(s);
    n->set_position(x, y);
    return n;
}
