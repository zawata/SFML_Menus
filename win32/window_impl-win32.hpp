#pragma once

#include "window_base.hpp"
#include "menubar.hpp"

struct NUI_Window_win32 : public NUI_Window {
    NUI_Window_win32(const std::string &s);
    NUI_Window_win32(const std::string &s, int w, int h);

    virtual void set_title(const std::string &s) override;
    virtual bool set_menubar(NUI_MenuBar &n) override;

    virtual void set_position(int x, int y) override;
    virtual void set_dimensions(int w, int h) override;

    virtual int run_loop() override;
private:
    
};