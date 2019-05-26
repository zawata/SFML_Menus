#include "window.hpp"
#include "win32/window_impl-win32.hpp"

NUI_Window *createWindow(const std::string &s) {
    return new NUI_Window_win32(s);
}

NUI_Window *createWindow(const std::string &s, int x, int y) {
    NUI_Window *n = new NUI_Window_win32(s);
    n->set_position(x, y);
    return n;
}