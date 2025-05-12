cmake_minimum_required(VERSION 3.10)

project(MonoEmbedDemo)

# Set paths
set(MONO_DIR ${CMAKE_SOURCE_DIR}/momo)
set(MONO_INCLUDE_DIR ${MONO_DIR}/include/mono-2.0)
set(MONO_LIB_DIR ${MONO_DIR}/lib)

# Output path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile the C# DLL using mcs
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Class1.dll
    COMMAND ${MONO_DIR}/bin/mcs -target:library -out:Class1.dll ${CMAKE_SOURCE_DIR}/Class1.cs
    DEPENDS ${CMAKE_SOURCE_DIR}/Class1.cs
    COMMENT "Compiling C# DLL"
)

# AOT compile the DLL
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Class1.dll.so
    COMMAND ${MONO_DIR}/bin/mono --aot=full ${CMAKE_CURRENT_BINARY_DIR}/Class1.dll
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Class1.dll
    COMMENT "AOT compiling C# DLL"
)

# Target to build the C# part
add_custom_target(build_cs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Class1.dll.so
)

# C++ executable
add_executable(MyApp main.cpp)
target_include_directories(MyApp PRIVATE ${MONO_INCLUDE_DIR})
target_link_directories(MyApp PRIVATE ${MONO_LIB_DIR})
target_link_libraries(MyApp PRIVATE monosgen-2.0 dl stdc++)

add_dependencies(MyApp build_cs)

# Post-build command to run the app
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env
        LD_LIBRARY_PATH=${MONO_LIB_DIR}
        # MONO_LOG_LEVEL=debug MONO_LOG_MASK=aot
        $<TARGET_FILE:MyApp>
    COMMENT "Running MyApp with Mono AOT"
)
