project('FormationMovement', 'c', 'cpp',
    version : '1.0',
    default_options : ['cpp_std = c++14', 'warning_level=3'])

dep_glew = dependency('glew')
dep_glfw = dependency('glfw3')

dir_includes = include_directories('include')

sources = [
    'include/stb_image.c',
    'src/Actor.cpp',
    'src/Angle.cpp',
    'src/App.cpp',
    'src/Camera.cpp',
    'src/Color.cpp',
    'src/common.cpp',
    'src/Flowfield.cpp',
    'src/Font.cpp',
    'src/Formation.cpp',
    'src/FormationBehavior.cpp',
    'src/HUD.cpp',
    'src/main.cpp',
    'src/Map.cpp',
    'src/Path.cpp',
    'src/Renderer.cpp',
    'src/Texture.cpp',
    'src/Unit.cpp',
    'src/UnitSelection.cpp',
    'src/Vector.cpp',
    'src/Window.cpp',
    'src/World.cpp',
]

executable('FormationMovement', sources,
    dependencies : [dep_glew, dep_glfw],
    include_directories : dir_includes,
    install : true)

# Make sure the data folder ends up in the same directory as the binary.
install_subdir('data', install_dir : '/bin')

build_dir = meson.current_build_dir()