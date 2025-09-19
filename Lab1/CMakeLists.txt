cmake_minimum_required(VERSION 3.20)
project(Lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only set SFML_DIR when building on Windows
if (WIN32)
    set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
endif()

# Require SFML 2.6.x (don’t use EXACT so 2.6.0 / 2.6.2 also work)
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

# Collect all source files
file(GLOB_RECURSE LAB1_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/code/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/code/*.h"
)

add_executable(BuzzyDefender ${LAB1_SOURCES})

target_link_libraries(BuzzyDefender PRIVATE sfml-system sfml-window sfml-graphics)

target_include_directories(BuzzyDefender PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code)

# Windows-only: copy SFML DLLs
if (WIN32)
    add_custom_command(TARGET BuzzyDefender POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/SFML-2.6.1/bin"
            "$<TARGET_FILE_DIR:BuzzyDefender>"
    )
endif()

# Copy graphics folder (works everywhere)
add_custom_command(TARGET BuzzyDefender POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/graphics"
        "$<TARGET_FILE_DIR:BuzzyDefender>/graphics"
)
