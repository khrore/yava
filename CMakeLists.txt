cmake_minimum_required(VERSION 3.28)

project(VulkanTutor VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE MAIN_SOURCES ./src/*.cxx)

add_executable(VulkanTutor ${MAIN_SOURCES})

target_link_libraries(VulkanTutor glfw glm Vulkan::Vulkan)

target_include_directories(VulkanTutor PUBLIC
  "${PROJECT_BINARY_DIR}/../src"
  "${PROJECT_BINARY_DIR}/../libs"
  "${PROJECT_BINARY_DIR}/../libs/glfw/include")
