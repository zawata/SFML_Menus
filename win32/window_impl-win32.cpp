#include "window.hpp"

#define NOMINMAX                        // Turn off min and max macros, they conflict std::min/max
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

/**
 * String Conversion
 **/
template <typename Facet>
struct deletable_facet : Facet { using Facet::Facet; };
std::wstring_convert<deletable_facet<std::codecvt<wchar_t, char, std::mbstate_t>>> conv;
std::string w_to_s(std::wstring w) { return conv.to_bytes(w); }
std::wstring s_to_w(std::string s) { return conv.from_bytes(s); }

/**
 * Constants
 **/
static const std::wstring class_name = L"SFML_WRAPPER";

/**
 * Static Functions
 **/
static HWND InitInstance(std::wstring window_title) {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    HWND hWnd = CreateWindowW(
        class_name.c_str(),
        window_title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
        0, 
        CW_USEDEFAULT, 
        0, 
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr);

   if (!hWnd) return NULL;

   ShowWindow(hWnd, SW_SHOW);
   UpdateWindow(hWnd);

   return hWnd;
}

static HWND InitSFML(SFML::RenderWindow sfml_window, HWND parent) {
    RECT parent_rect;
    GetWindowRect(parent, &parent_rect);

    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Create an SFML View
    HWND view = CreateWindow(
        L"STATIC",
        L"",
        WS_CHILD | WS_VISIBLE,
        0,
        0,
        parent_rect.right - parent_rect.left,
        parent_rect.bottom - parent_rect.top,
        parent,
        NULL,
        hInstance,
        NULL);

    if (!view) return NULL;

    sfml_window.create(view, sf::ContextSettings(24));

    return view;
}

static ATOM MyRegisterClass() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = L"SFML_WRAPPER";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

/**
 * NUI_Window_win32 Implementation
 **/
NUI_Window_win32::NUI_Window_win32(const std::string &s) {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    MyRegisterClass();

    main_hwnd = InitInstance(s_to_w(s));
    sfml_hwnd = InitSFML(main_hwnd);

    SetFocus(sfml_hwnd);
}

NUI_Window_win32::NUI_Window_win32(const std::string &s, int w, int h) {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    MyRegisterClass();

    main_hwnd = InitInstance(s_to_w(s));
    sfml_hwnd = InitSFML(main_hwnd);

    SetFocus(sfml_hwnd);
    this->set_size(w, h);
}

void NUI_Window_win32::set_title(const std::string &s) {
    SetWindowTextW(this->main_hwnd, s_to_w(s));
}

bool NUI_Window_win32::set_menubar(NUI_MenuBar &n) {
    if(menubar_locked) return false;

    std::deque<std::pair<HWND, NUI_MenuItem *>> d;

    HWND menubar = createMenu();
    for(auto i : *menu) d.push_back({menubar, i});

    while(!d.empty()) {
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

        gtk_menu->append_item(menu_item);
    }
    std::cout << "setting" << std::endl;
    this->set_menubar(menubar);

    return true;
}

void NUI_Window_win32::set_position(int x, int y) {
    RECT bounds;
    GetWindowRect(parent, &bounds);

    int w = bounds.bottom - bounds.top,
        h = bounds.right - bounds.left;

    MoveWindow(this->main_hwnd,
        x,
        y,
        x+w,
        y+h);
}

void NUI_Window_win32::set_dimensions(int w, int h) {
    RECT bounds;
    GetWindowRect(parent, &bounds);

    MoveWindow(this->main_hwnd,
        bounds.top,
        bounds.left,
        bounds.top + w,
        bounds.left + h);
}


int NUI_Window_win32::run_loop() {
    MSG msg;
    do {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            sf::Event Event;
            while(sfml_window.pollEvent(Event)) {
                //TODO: onSFMLEvent();
                if(Event.type == sf::Event::KeyPressed) {
                    if(Event.key.code == sf::Keyboard::Key::Enter) {
                        return false;
                    }
                }
            }

            //TODO: onDraw();

            sfml_window.clear();
            sfml_window.draw(sBackground);
            sfml_window.display();
        }
    } while (msg.message != WM_QUIT);

    return (int) msg.wParam;
}