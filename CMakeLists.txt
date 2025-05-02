cmake_minimum_required(VERSION 3.0)
project(MonoAOTExample)

# Mono per pkg-config finden (gegebenenfalls PKG_CONFIG_PATH setzen)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MONO mono-2)

include_directories(${MONO_INCLUDE_DIRS})
link_directories(${MONO_LIBRARY_DIRS})

add_executable(main main.cpp)
target_link_libraries(main ${MONO_LIBRARIES} dl)
