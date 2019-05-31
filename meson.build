project('main', ['c', 'cpp'],
    meson_version: '>=0.50.0',
    default_options: [
        'cpp_std=c++11'
    ])

sources = [
    'test_main.cpp',
]

dependencies = []

system = host_machine.system()
if system == 'windows'
    sources += [
        'win32/window-win32.cpp',
        'win32/window_impl-win32.cpp'
    ]

    add_project_arguments(
        '-D_IS_WINDOWS',
        language: ['c', 'cpp'])

elif system == 'darwin'
    sources += [
        'macos/window-cocoa.cpp',
        'macos/window_impl-cocoa.cpp'
    ]

    add_project_arguments(
        '-D_IS_MACOS',
        language: ['c', 'cpp'])
elif system == 'linux'
    sources += [
        'gtk/window-gtk.cpp',
        'gtk/window_impl-gtk.cpp',
        'gtk/sfml_widget.cpp'
    ]

    dependencies += [
        dependency('gtkmm-3.0')
    ]

    add_project_arguments(
        '-D_IS_LINUX',
        language: ['c', 'cpp'])
else
    # if you get here, you'll get linker errors
endif

dependencies += [
    dependency('sfml-all')
]

executable('main', sources,
    gui_app: true,
    dependencies: dependencies)
