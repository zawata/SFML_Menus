#pragma once

#include <list>
#include <iostream>

#define ID_COUNTER_START 40500

struct NUI_MenuItem {
    virtual ~NUI_MenuItem() {}

    virtual bool is_SubMenuItem()       { return false; }
    virtual bool is_TextMenuItem()      { return false; }
    virtual bool is_ToggleMenuItem()    { return false; }
    virtual bool is_CallbackMenuItem()  { return false; }
    virtual bool is_SeperatorMenuItem() { return false; }

    virtual bool operator==(NUI_MenuItem &n) const { return get_id() == n.get_id(); }
    virtual bool operator!=(NUI_MenuItem &n) const { return !operator==(n); }

    int get_id() const { return item_id; }

protected:
    NUI_MenuItem() :
    item_id(__id()) {
    }

private:
    static int __id() {
        static int id = ID_COUNTER_START;
        return id++;
    }

    int item_id;
};

#define TextMenuItem(...) new NUI_TextMenuItem(__VA_ARGS__)
struct NUI_TextMenuItem : public NUI_MenuItem {

    NUI_TextMenuItem(const std::string &label) :
    label(label) {}

    virtual bool is_TextMenuItem() override { return true; }

    virtual bool operator==(NUI_MenuItem &n) const override {
        if(n.is_TextMenuItem()) {
            return ((NUI_TextMenuItem &)n).label == this->label;
        }
        return false;
     }

    const std::string &label;
};

// #define SeperatorMenuItem(...) new NUI_SeperatorMenuItem(__VA_ARGS__)
// struct NUI_SeperatorMenuItem : public NUI_MenuItem {
//     NUI_SeperatorMenuItem();
//
//     virtual bool is_SeperatorMenuItem() { return true; }
// };

#define SubMenuItem(...) new NUI_SubMenuItem(__VA_ARGS__)
struct NUI_SubMenuItem : public NUI_TextMenuItem, public std::list<NUI_MenuItem *> {
    NUI_SubMenuItem(const std::string &label, std::initializer_list<NUI_MenuItem *> items) :
    NUI_TextMenuItem(label),
    std::list<NUI_MenuItem *>(items) {}

    //TODO destructor to delete list of pointers

    virtual bool is_SubMenuItem() override { return true; }
};

#define ToggleMenuItem(...) new NUI_ToggleMenuItem(__VA_ARGS__)
struct NUI_ToggleMenuItem : public NUI_TextMenuItem {
    NUI_ToggleMenuItem(const std::string &label, bool *state) :
    NUI_TextMenuItem(label),
    callback_mode(false),
    state_toggle(state) {}

    NUI_ToggleMenuItem(const std::string &label, std::function<void(NUI_ToggleMenuItem &, bool, void *)> cb, void *us_data = nullptr, bool initial_state = false) :
    NUI_TextMenuItem(label),
    cb(cb),
    callback_mode(true),
    default_state(initial_state),
    us_data(us_data) {}

    void operator()(bool new_state) {
        if(callback_mode)
            cb(*this, new_state, us_data);
        else {
            if(state_toggle)
                *state_toggle = !*state_toggle;
        }
    }

    bool get_default_state() {
        return default_state;
    }

    virtual bool is_ToggleMenuItem() override { return true; }
private:
    std::function<void(NUI_ToggleMenuItem &, bool, void *us_Data)> cb;
    bool callback_mode,
         *state_toggle,
         default_state;
    void *us_data;
};

#define CallbackMenuItem(...) new NUI_CallbackMenuItem(__VA_ARGS__)
struct NUI_CallbackMenuItem : public NUI_TextMenuItem {
    NUI_CallbackMenuItem(const std::string &label, std::function<void(NUI_CallbackMenuItem &, void *)> cb, void *us_data = nullptr) :
    NUI_TextMenuItem(label),
    cb(cb),
    us_data(us_data) {}

    void operator()() {
        cb(*this, us_data);
    }

    virtual bool is_CallbackMenuItem() override { return true; }
private:
    std::function<void(NUI_CallbackMenuItem &, void *us_Data)> cb;
    void *us_data;
};

struct NUI_MenuBar : public std::list<NUI_SubMenuItem *> {
    NUI_MenuBar(std::initializer_list<NUI_SubMenuItem *> items) :
    std::list<NUI_SubMenuItem *>(items) {}

    //TODO destructor to delete list of pointers
};