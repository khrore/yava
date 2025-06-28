cmake_minimum_required(VERSION 3.28)

project(VulkanTutor VERSION 0.1 LANGUAGES CXX C)

set(PROJECT_NAME VulkanTutor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE MAIN_SOURCES src/*.cxx)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw glm Vulkan::Vulkan)

target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_CURRENT_BINARY_DIR}/../src"
  "${CMAKE_CURRENT_BINARY_DIR}/../src/"
  "${CMAKE_CURRENT_BINARY_DIR}/../libs"
  "${CMAKE_CURRENT_BINARY_DIR}/../libs/glfw/include")


function(compile_shaders)
    message("-- Compiling shaders.")
    
    file(GLOB_RECURSE SHADERS_FRAG RELATIVE ${CMAKE_CURRENT_BINARY_DIR}/../assets/shaders/ assets/shaders/*.frag)
    file(GLOB_RECURSE SHADERS_VERT RELATIVE ${CMAKE_CURRENT_BINARY_DIR}/../assets/shaders/ assets/shaders/*.vert )
    file(MAKE_DIRECTORY assets/shaders)

    foreach(SHADER_FRAG SHADER_VERT IN ZIP_LISTS SHADERS_FRAG SHADERS_VERT)
        execute_process(COMMAND glslc ../assets/shaders/${SHADER_FRAG} -o assets/shaders/${SHADER_FRAG}.spv)
        execute_process(COMMAND glslc ../assets/shaders/${SHADER_VERT} -o assets/shaders/${SHADER_VERT}.spv)
    endforeach()
endfunction()

function(copy_textures)
    message("-- Coping textures.")

    file(GLOB_RECURSE TEXTURES RELATIVE ${CMAKE_CURRENT_BINARY_DIR}/../assets/textures/ assets/textures/*.*)
    file(MAKE_DIRECTORY build/assets/textures)

    foreach(TEXTURE ${TEXTURES})
        file(COPY_FILE assets/textures/${TEXTURE} build/assets/textures/${TEXTURE})
    endforeach()
endfunction()


function(copy_models)
    message("-- Coping models.")

    file(GLOB_RECURSE MODELS RELATIVE ${CMAKE_CURRENT_BINARY_DIR}/../assets/models/ assets/models/*.*)
    file(MAKE_DIRECTORY build/assets/models)

    foreach(MODEL ${MODELS})
        file(COPY_FILE assets/models/${MODEL} build/assets/models/${MODEL})
    endforeach()
endfunction()

if (CMPSDR)
    compile_shaders()
endif()

if (CPTX)
    copy_textures()
endif()

if (CPMODS)
    copy_models()
endif()