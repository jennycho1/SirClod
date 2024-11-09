cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # sets .lib files to be output here
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # sets .exe files to be output here

set(PROJECT_NAME SirClod)
project(${PROJECT_NAME} 
    VERSION 0.0.0
    DESCRIPTION "Vulkan Rendering Engine"
    LANGUAGES CXX
)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE 
src/main.cpp 
src/vulkan_engine.cpp
src/vulkan_engine.h
)

add_subdirectory(external/volk)
target_link_libraries(${PROJECT_NAME} PRIVATE volk)
target_include_directories(${PROJECT_NAME} PRIVATE external/volk)

add_subdirectory(external/vkbootstrap)
target_link_libraries(${PROJECT_NAME} PRIVATE vk-bootstrap::vk-bootstrap)
target_include_directories(${PROJECT_NAME} PRIVATE external/vkbootstrap)

target_include_directories(${PROJECT_NAME} PRIVATE external/SDL2/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/SDL2/lib/x64)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2main)