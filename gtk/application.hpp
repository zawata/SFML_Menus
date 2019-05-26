#pragma once

#include <gtkmm/application.h>
#include <gtkmm/menubar.h>

#include <iostream>
#include <cassert>

#include "window.hpp"
#include "menubar.hpp"


class NUI_Application : public Gtk::Application {
public:
    static NUI_Application *create(const std::string &id) {
        return new NUI_Application(id);
    }

    void add_MenuBar(NUI_MenuBar &n) {
        this->menu = new NUI_MenuBar(n);
    }

protected:
    NUI_Application(const std::string &id) :
    Gtk::Application(id),
    menu(nullptr) {}

    // Override default signal handlers:
    void on_activate() override {
        create_appwindow()->present();
    }

    void on_startup() override {
        Gtk::Application::on_startup();
        if(menu) create_MenuBar();
    }

    void create_MenuBar() {
        std::deque<std::pair<Glib::RefPtr<Gio::Menu>, NUI_MenuItem *>> d;

        auto menubar = Gio::Menu::create();
        for(auto i : *menu) d.push_back({menubar, i});

        while(!d.empty()) {
            std::cout << "not empty" << std::endl;
            Glib::RefPtr<Gio::Menu> gtk_menu = d.front().first;
            NUI_MenuItem *nui_m = d.front().second;
            d.pop_front();

            std::string label= ((NUI_TextMenuItem *)nui_m)->title;
            auto menu_item = Gio::MenuItem::create("_" + label, "win." + label);

            if(nui_m->is_SubMenuItem()) {
                auto sub_menu = Gio::Menu::create();
                menu_item->set_submenu(sub_menu);
                for(auto i : *(NUI_SubMenuItem *)nui_m) d.push_back({sub_menu, i});
            }

            // if(nui_m->is_TextMenuItem()) {
            //     menu_item.set_label(((NUI_TextMenuItem *)nui_m)->title);
            // }

            gtk_menu->append_item(menu_item);
        }
        std::cout << "setting" << std::endl;
        this->set_menubar(menubar);
    }

private:
    NUI_Window *create_appwindow() {
        auto appwindow = new NUI_Window();

        add_window(*appwindow);

        return appwindow;
    }

    bool initialized;
    NUI_MenuBar *menu;
};