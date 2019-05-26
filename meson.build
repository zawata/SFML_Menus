project('main', ['c', 'cpp'],
    meson_version: '>=0.50.0')

sources = [
    'main.cpp',
]

dependencies = [
    dependency('sfml-all')
]

system = host_machine.system()
if system == 'windows'
    sources += [
        'win32/window-win32.cpp',
        'win32/window_impl-win32.cpp'
    ]
elif system == 'darwin'
    sources += [
        'macos/window-cocoa.cpp',
        'macos/window_impl-cocoa.cpp'
    ]
elif system == 'linux'
    sources += [
        'gtk/window-gtk.cpp',
        'gtk/window_impl-gtk.cpp',
        'gtk/sfml_widget.cpp'
    ]

    dependencies += [
        dependency('gtkmm-3.0')
    ]
else
    # if you get here, you'll get linker errors
endif

executable('main', sources,
    gui_app: true,
    dependencies: dependencies)