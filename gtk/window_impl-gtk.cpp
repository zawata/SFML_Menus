#include <glibmm/main.h>
#include "gtk/window_impl-gtk.hpp"


#include <SFML/Graphics.hpp>

static const std::string class_name = "org.zawata.SFML_WRAPPER";

NUI_Window_gtk::NUI_Window_gtk(const std::string &title) :
Gtk::Application(class_name),
menu(nullptr),
main_window(new Gtk::ApplicationWindow()) {
    this->set_title(title);
}

NUI_Window_gtk::NUI_Window_gtk(const std::string &title, int w, int h) :
Gtk::Application(class_name),
menu(nullptr),
main_window(nullptr) {
    this->set_title(title);
    this->set_dimensions(w, h);
}

void NUI_Window_gtk::set_title(const std::string &s) {
    main_window->set_title(s);
}

bool NUI_Window_gtk::add_menubar(NUI_MenuBar *n) {
    menu = n;
    return !initialized;
}

void NUI_Window_gtk::set_position(int x, int y) {
    main_window->move(x, y);
}

void NUI_Window_gtk::set_dimensions(int w, int h) {
    main_window->resize(w, h);
}

void NUI_Window_gtk::set_on_event_callback(event_callback_t c) {
    this->on_event_callback = c;
}

void NUI_Window_gtk::set_on_draw_callback(draw_callback_t c) {
    this->on_draw_callback = c;
}

void NUI_Window_gtk::set_on_menu_callback(menu_callback_t c) {
    this->on_menu_callback = c;
}

int NUI_Window_gtk::run_loop() {
    int w, h;
    main_window->get_size(w,h);
    sfml_widget = new SFMLWidget(sf::VideoMode(w, h));
    main_window->add(*sfml_widget);

    Glib::signal_idle().connect(
        sigc::bind<0>(
            sigc::mem_fun(*sfml_widget, &SFMLWidget::run_events),
            on_event_callback));

    return this->run(*main_window);
}


// Override default signal handlers:
void NUI_Window_gtk::on_activate() {
    Gtk::Application::on_activate();
    main_window->present();
}

void NUI_Window_gtk::on_startup() {
    Gtk::Application::on_startup();
    this->add_window(*main_window);

    if(menu) set_menubar_gtk();
    initialized = true;
}

void NUI_Window_gtk::set_menubar_gtk() {
    //deque for a breadth-first tree traversal
    std::deque<std::pair<Glib::RefPtr<Gio::Menu>, NUI_MenuItem *>> d;

    auto menubar = Gio::Menu::create();
    for(auto i : *menu) d.push_back({menubar, i});

    while(!d.empty()) {
        Glib::RefPtr<Gio::Menu> gtk_menu = d.front().first;
        NUI_MenuItem *nui_m = d.front().second;
        d.pop_front();

        std::string label = ((NUI_TextMenuItem *)nui_m)->label;
        std::string action_name = std::to_string(nui_m->get_id()) + "." + label;

        if(nui_m->is_SubMenuItem()) {
            auto sub_menu = Gio::Menu::create();
            gtk_menu->append_submenu(label, sub_menu);
            for(auto i : *(NUI_SubMenuItem *)nui_m) d.push_back({sub_menu, i});
        } else {
            gtk_menu->append(label, "app." + action_name);

            if(nui_m->is_ToggleMenuItem()) {
                Glib::RefPtr<Gio::SimpleAction> menu_item_action = Gio::SimpleAction::create_bool(action_name);
                menu_item_action->signal_change_state().connect([nui_m, menu_item_action](const Glib::VariantBase &b) -> void {
                    menu_item_action->set_state(b);

                    //Damn you variant types
                    bool new_state = Glib::VariantBase::cast_dynamic<Glib::Variant<bool>>(b).get();

                    (*(NUI_ToggleMenuItem *)nui_m)(new_state);
                });

                Gtk::Application::add_action(menu_item_action);
            }
            else if(nui_m->is_CallbackMenuItem()) {
                std::cout << "adding callback" << std::endl;
                Glib::RefPtr<Gio::SimpleAction> menu_item_action = Gio::SimpleAction::create(action_name);
                menu_item_action->signal_activate().connect(sigc::hide([nui_m]() -> void {
                    (*(NUI_CallbackMenuItem *)nui_m)();
                }));

                Gtk::Application::add_action(menu_item_action);
            }
            // else if(nui_m->is_RadioMenuItem()) {
            //     TODO: Radio Menu Items...string or int types?
            // }
        }
    }

    this->set_menubar(menubar);
}