project('FormationMovement', 'c', 'cpp',
    version : '1.0',
    default_options : ['cpp_std = c++14', 'warning_level=3'])

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

dep_glew = dependency('glew')
dep_glfw = dependency('glfw3')

executable('FormationMovement', sources,
    dependencies : [dep_glew, dep_glfw],
    include_directories : dir_includes,
    install : true)

build_dir = meson.current_build_dir()

fs = import('fs')

# Not sure if this is the correct way to copy DLLs.
glew_dll_src = build_dir / 'subprojects/glew-2.2.0/glew32.dll'
fs.copyfile(glew_dll_src)

glfw_dll_src = build_dir / 'subprojects/glfw-3.3.10/glfw3-3.dll'
fs.copyfile(glfw_dll_src)