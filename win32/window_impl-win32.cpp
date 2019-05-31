#include "win32/window_impl-win32.hpp"

#include <deque>

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
HWND NUI_Window_win32::initInstance(std::wstring window_title) {
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

   return hWnd;
}

HWND NUI_Window_win32::initSFML(sf::RenderWindow &sfml_window, HWND parent) {
    RECT parent_rect;
    GetWindowRect(parent, &parent_rect);

    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Create an SFML View
    HWND view = CreateWindow(
        "STATIC",
        "",
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

ATOM NUI_Window_win32::registerClass() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = NUI_Window_win32::wndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = 0;
    wcex.hbrBackground  = 0;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = class_name.c_str();

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK NUI_Window_win32::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return ((NUI_Window_win32 *)(void *)GetWindowLongPtrA(hWnd, GWLP_USERDATA))->on_win32_event(hWnd, message, wParam, lParam);
}

/**
 * NUI_Window_win32 Implementation
 **/
NUI_Window_win32::NUI_Window_win32(const std::string &s) {

    registerClass();

    main_hwnd = initInstance(s_to_w(s));
    sfml_hwnd = initSFML(this->sfml_window, main_hwnd);

    SetWindowLongPtrA(main_hwnd, GWLP_USERDATA, (LONG_PTR)(void *)this);

	ShowWindow(main_hwnd, SW_SHOW);
	UpdateWindow(main_hwnd);

    SetFocus(sfml_hwnd);
}

NUI_Window_win32::NUI_Window_win32(const std::string &s, int w, int h) {
    registerClass();

    main_hwnd = initInstance(s_to_w(s));
    sfml_hwnd = initSFML(this->sfml_window, main_hwnd);

	ShowWindow(main_hwnd, SW_SHOW);
	UpdateWindow(main_hwnd);

	this->set_dimensions(w, h);

    SetFocus(sfml_hwnd);
}

void NUI_Window_win32::set_title(const std::string &s) {
    SetWindowTextW(this->main_hwnd, s_to_w(s).c_str());
}

bool NUI_Window_win32::add_menubar(NUI_MenuBar *menu_layout) {
    //deque for a breadth-first tree traversal
    std::deque<std::pair<HMENU, NUI_MenuItem *>> d;

    HMENU main_menubar = CreateMenu();
    for(auto i : *menu_layout) d.push_back({main_menubar, i});

    while(!d.empty()) {
        HMENU win_menu = d.front().first;
        NUI_MenuItem *nui_m = d.front().second;
        d.pop_front();

        std::string label = ((NUI_TextMenuItem *)nui_m)->label;
        //std::string action_name = std::to_string(nui_m->get_id()) + "." + label;

        if(nui_m->is_SubMenuItem()) {
            HMENU sub_menu = CreatePopupMenu();
            AppendMenu(win_menu, MF_POPUP | MF_STRING, (UINT_PTR)(void *)sub_menu, label.c_str());
            for(auto i : *(NUI_SubMenuItem *)nui_m) d.push_back({sub_menu, i});
        } else if(nui_m->is_ToggleMenuItem()) {
            int item_pos = GetMenuItemCount(win_menu);
            int default_state = (((NUI_ToggleMenuItem *)nui_m)->get_default_state()) ? MF_CHECKED : MF_UNCHECKED;
            AppendMenu(win_menu, MF_STRING | default_state, nui_m->get_id(), label.c_str());

            callback_map.insert({nui_m->get_id(), std::make_pair(nui_m, [=](NUI_MenuItem *nui_m) {
                bool current_state = GetMenuState(win_menu, item_pos, MF_BYPOSITION) & MF_CHECKED;
                (*(NUI_ToggleMenuItem *)nui_m)(!current_state);
                CheckMenuItem(win_menu, item_pos, MF_BYPOSITION | ((!current_state) ? MF_CHECKED : MF_UNCHECKED));
            })});
        } else if(nui_m->is_CallbackMenuItem()) {
            AppendMenu(win_menu, MF_STRING, nui_m->get_id(), label.c_str());
            callback_map.insert({nui_m->get_id(), std::make_pair(nui_m, [](NUI_MenuItem *nui_m){
                (*(NUI_CallbackMenuItem *)nui_m)();
            })});
        }
    }

    SetMenu(main_hwnd, main_menubar);
    return true;
}

void NUI_Window_win32::set_position(int x, int y) {
    RECT bounds;
    GetWindowRect(main_hwnd, &bounds);

    int w = bounds.bottom - bounds.top,
        h = bounds.right - bounds.left;

    MoveWindow(this->main_hwnd,
        x,
        y,
        x+w,
        y+h,
        FALSE);
}

void NUI_Window_win32::set_dimensions(int w, int h) {
    RECT bounds;
    GetWindowRect(main_hwnd, &bounds);

    MoveWindow(this->main_hwnd,
        bounds.top,
        bounds.left,
        bounds.top + w,
        bounds.left + h,
        FALSE);
}

void NUI_Window_win32::set_on_event_callback(event_callback_t c) {
    this->on_event_callback = c;
}

void NUI_Window_win32::set_on_draw_callback(draw_callback_t c) {
    this->on_draw_callback = c;
}

void NUI_Window_win32::set_on_menu_callback(menu_callback_t c) {
    this->on_menu_callback = c;
}

void NUI_Window_win32::message_box(std::string &title, std::string &message) {
    MessageBox(main_hwnd, message.c_str(), title.c_str(), MB_OK);   
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
                this->on_event_callback(Event);
            }

            sfml_window.clear();
            this->on_draw_callback();
            sfml_window.display();
        }
    } while (msg.message != WM_QUIT);

    return (int) msg.wParam;
}

LRESULT NUI_Window_win32::on_win32_event(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        try {
            auto menu_comp = callback_map.at(wmId);
            menu_comp.second(menu_comp.first);
        } catch(std::out_of_range &) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }   break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }   break;
    //case WM_SIZE:
    case WM_SIZING: {
        RECT size = *(LPRECT)lParam;
        SetWindowPos(
            sfml_hwnd,
            HWND_TOP,
            0, 0,
            size.right - size.left,
            size.bottom - size.top,
            SWP_SHOWWINDOW);

        sfml_window.setView(
            sf::View(sf::FloatRect(
                0,
                0,
                size.right - size.left,
                size.bottom - size.top)));

    }   break;
    case WM_ACTIVATE:
        SetFocus(sfml_hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
