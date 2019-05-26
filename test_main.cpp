#include <iostream>

#include "window.hpp"
#include "menubar.hpp"

auto on_open = [](NUI_CallbackMenuItem &, void *) {
    std::cout << "open" << std::endl;
};

auto on_toggle = [](NUI_ToggleMenuItem &, bool new_state, void *) {
    std::cout << "Toggle: " << new_state << std::endl;
};

int main (int argc, char *argv[]) {
    auto application = createWindow("New Window");

    auto menu_bar = new NUI_MenuBar({
        SubMenuItem("File", {
            CallbackMenuItem("Open", on_open),
        }),
        SubMenuItem("Edit", {
            ToggleMenuItem("Toggle", on_toggle),
            TextMenuItem("Other"),
        }),
    });
    application->add_menubar(menu_bar);

    return application->run_loop();
}