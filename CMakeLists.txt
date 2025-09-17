cmake_minimum_required(VERSION 3.21)
project(game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Source files
add_executable(game
    src/main.cpp
)

# Find SFML 3 via vcpkg (CONFIG mode)
find_package(SFML 3 COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)

target_link_libraries(game PRIVATE
    SFML::Network
    SFML::Graphics
    SFML::Window
    SFML::Audio
    SFML::System
)

# On macOS, bundle resources nicely (optional)
if(APPLE)
    set_target_properties(game PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.yourgame"
    )
endif()
