#pragma once

#include <SFML/Graphics.hpp>
#include <gtkmm/widget.h>

#include <gdk/gdkx.h>
#define GET_WINDOW_HANDLE_FROM_GDK GDK_WINDOW_XID

class SFMLWidget : public Gtk::Widget {
protected:
    sf::VideoMode m_vMode;
    Glib::RefPtr<Gdk::Window> m_refGdkWindow;

    void on_size_allocate(Gtk::Allocation& allocation) override;
    void on_realize() override;
    void on_unrealize() override;

    //virtual Gtk::SizeRequestMode get_request_mode_vfunc() const override;
    void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const override;
    void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const  override;
    void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const override;
    void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const override;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

public:
    SFMLWidget(sf::VideoMode mode, int size_request=-1);

    bool run_events(std::function<void(sf::Event)> f);

    virtual ~SFMLWidget();

    void invalidate();
    void display();

    sf::RenderWindow renderWindow;
};

//  X  get_preferred_width_vfunc(): Calculate the minimum and natural width of the widget.
//  X  get_preferred_height_vfunc(): Calculate the minimum and natural height of the widget.
//  X  get_preferred_width_for_height_vfunc(): Calculate the minimum and natural width of the widget, if it would be given the specified height.
//  X  get_preferred_height_for_width_vfunc(): Calculate the minimum and natural height of the widget, if it would be given the specified width.
//  X  on_size_allocate(): Position the widget, given the height and width that it has actually been given.
//  X  on_realize(): Associate a Gdk::Window with the widget.

//     get_request_mode_vfunc(): (optional) Return what Gtk::SizeRequestMode is preferred by the widget.
//  X  on_unrealize(): (optional) Break the association with the Gdk::Window.
//     on_map(): (optional)
//     on_unmap(): (optional)
    
