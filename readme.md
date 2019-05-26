## SFML_Menus

Native Menus + SFML

Basically just a framework wrapper for Windows and Menus in GTK, win32 and Cocoa

Mostly done now because I like the prospect of libui but the API is so volatile the c++ extension gave up. Once it stabilizes, it'd be pretty trivial-ish to add SFML Widgets.  

### Files:  
 - window.hpp: Window Creation Function forward declarations
 - window_base.hpp: NUI_Window Base Declaration
 - window-\<platform>.cpp: platform-specific Window Creation Function definitions
 - window_impl-\<platform>.hpp platform-specific NUI_Window declaration
 - window_impl-\<platform>.cpp platform-specific NUI_Window implementation

### Status:
 - gtk
   - Windows
     - ~~SFML~~
   - Menus
     - Items
        - SubMenus
        - Text Items
        - Toggle Items
        - Callback Items
        - ~~Separators~~
     - ~~enable/disable~~
 - win32
   - Windows
     - SFML
   - ~~Menus~~
 - ~~cocoa~~

### Example:
```cpp
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
```