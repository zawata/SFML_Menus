## SFML_Menus

Native Menus + SFML

Basically just a framework wrapper for Windows and Menus in GTK, win32 and Cocoa

Mostly done now because I like the prospect of libui but the API is so volatile the c++ extension gave up. Once it stabilizes, it'd be pretty trivial-ish to add SFML Widgets.  

Files:  
 - window.hpp: Window Creation Function forward declarations
 - window_base.hpp: NUI_Window Base Declaration
 - window-\<platform>.cpp: platform-specific Window Creation Function definitions
 - window_impl-\<platform>.hpp platform-specific NUI_Window declaration
 - window_impl-\<platform>.cpp platform-specific NUI_Window implementation