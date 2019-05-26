#include "gtk/sfml_widget.hpp"

void SFMLWidget::on_size_allocate(Gtk::Allocation& allocation) {
    //Do something with the space that we have actually been given:
    //(We will not be given heights or widths less than we have requested, though
    //we might get more)

    this->set_allocation(allocation);

    if(m_refGdkWindow) {
        m_refGdkWindow->move_resize(allocation.get_x(),
                                    allocation.get_y(),
                                    allocation.get_width(),
                                    allocation.get_height() );
        renderWindow.setSize(sf::Vector2u(allocation.get_width(),
                                        allocation.get_height()));
    }
}

void SFMLWidget::on_realize() {
    Gtk::Widget::on_realize();

    if(!m_refGdkWindow) {
        //Create the GdkWindow:
        GdkWindowAttr attributes;
        memset(&attributes, 0, sizeof(attributes));

        Gtk::Allocation allocation = get_allocation();

        //Set initial position and size of the Gdk::Window:
        attributes.x = allocation.get_x();
        attributes.y = allocation.get_y();
        attributes.width = allocation.get_width();
        attributes.height = allocation.get_height();

        attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;


        m_refGdkWindow = Gdk::Window::create(get_window(), &attributes,
                GDK_WA_X | GDK_WA_Y);
        set_has_window(true);
        set_window(m_refGdkWindow);

        // transparent background
#if GTK_VERSION_GE(3, 0)
        this->unset_background_color();
#else
        this->get_window()->set_back_pixmap(Glib::RefPtr<Gdk::Pixmap>());
#endif

        this->set_double_buffered(false);

        //make the widget receive expose events
        m_refGdkWindow->set_user_data(gobj());

        renderWindow.create(GET_WINDOW_HANDLE_FROM_GDK(m_refGdkWindow->gobj()));
    }
}

void SFMLWidget::on_unrealize() {
    m_refGdkWindow.clear();

    //Call base class:
    Gtk::Widget::on_unrealize();
}

void SFMLWidget::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const {
    minimum_width = 0;
    natural_width = 0;
}

void SFMLWidget::get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const {
    minimum_height = 0;
    natural_height = 0;
}

void SFMLWidget::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const {
    minimum_height = 0;
    natural_height = 0;
}

void SFMLWidget::get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const {
    minimum_width = 0;
    natural_width = 0;
}

bool SFMLWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    return false;
}

SFMLWidget::SFMLWidget(sf::VideoMode mode, int size_request):
Glib::ObjectBase("SFMLWidget"),
Gtk::Widget() {
    if(size_request<=0)
        size_request = std::max<int>(1, std::min<int>(mode.width, mode.height) / 2);

    set_size_request(size_request, size_request);

    set_has_window(false); // Makes this behave like an interal object rather then a parent window.
}

bool SFMLWidget::run_events(std::function<void(sf::Event)> f) {
    sf::Event e;
    while(renderWindow.pollEvent(e)) { f(e); }
    return true;
}

SFMLWidget::~SFMLWidget() {}

void SFMLWidget::invalidate() {
    if(m_refGdkWindow)
    {
        renderWindow.display();
    }
}

void SFMLWidget::display() {
    if(m_refGdkWindow)
    {
        renderWindow.display();
    }
}
