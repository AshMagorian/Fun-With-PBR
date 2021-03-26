cmake_minimum_required(VERSION 2.6)
project(VANILLA_ENGINE)

include_directories(
src
contrib
)

if(MSVC)
  include_directories(
    contrib/msvc-windows-x86/glew/include
    contrib/msvc-windows-x86/openal-soft/include
    contrib/msvc-windows-x86/ogg/include
    contrib/msvc-windows-x86/vorbis/include
    contrib/msvc-windows-x86/GLFW/include
    contrib/msvc-windows-x86/assimp/include
    contrib/imgui
  )

  link_directories(
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/ogg/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/vorbis/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/GLFW/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/assimp/lib
  )
endif()

add_subdirectory(contrib)
add_subdirectory(src)

add_executable(game
	src/game/main.cpp
	src/game/FirstPersonCameraControls.h
	src/game/FirstPersonCameraControls.cpp
)

target_link_libraries(game VanillaEngine)

if(MSVC)
  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
	  "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/bin/glew32.dll"
	  "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/bin/OpenAL32.dll"
	  "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/GLFW/bin/glfw3.dll"
	  "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/assimp/bin/assimp-vc141-mtd.dll"
	  "$<TARGET_FILE_DIR:game>"
  )
endif()