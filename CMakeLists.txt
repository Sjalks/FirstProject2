cmake_minimum_required(VERSION 3.21)
project(game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Fetch SFML 3 (no package manager) ---
include(FetchContent)
FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.0.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SFML)

# --- Your program ---
add_executable(game src/main.cpp)

target_link_libraries(game PRIVATE
  SFML::System
  SFML::Window
  SFML::Graphics
  SFML::Audio
  SFML::Network
)

# --- Windows: copy required DLLs next to the .exe so it runs ---
if (WIN32)
  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:game> $<TARGET_FILE_DIR:game>
    COMMAND_EXPAND_LISTS
  )
endif()
