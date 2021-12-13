cmake_minimum_required(VERSION 3.15)

project(grafikk-eksamen)

find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)

add_executable(${PROJECT_NAME}
    main.cpp
    Sprite.cpp
    Sprite.h
    functions.cpp
    functions.h
    stb_image.h
    stb_image_c.cpp
    shaders/spriteShader.h
    Window.h
    Window.cpp)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_SOURCE_DIR}/shaders)

target_link_libraries(${PROJECT_NAME}
  glfw
  glad
  glm
  OpenGL::GL)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/assets/example.png
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/example.png)