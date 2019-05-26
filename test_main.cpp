#include <gtkmm.h>

#include <iostream>

#include "window.hpp"
#include "menubar.hpp"

int main (int argc, char *argv[]) {
    auto application = createWindow("New Window");

    auto menu_bar = new NUI_MenuBar({
        SubMenuItem("File", {
            CallbackMenuItem("Open", [](NUI_CallbackMenuItem &, void *) -> auto {
                std::cout << "open" << std::endl;
            }),
        }),
        SubMenuItem("Edit", {
            ToggleMenuItem("Toggle", [](NUI_ToggleMenuItem &, bool new_state, void *) -> auto {
                std::cout << "Toggle: " << new_state << std::endl;
            }),
            TextMenuItem("Other"),
        }),
    });
    application->add_menubar(menu_bar);

    return application->run_loop();
}