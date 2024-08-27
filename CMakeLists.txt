cmake_minimum_required(VERSION 3.20)
project(fluid_simulation VERSION "1.0.0" DESCRIPTION "Fluid Simulation" LANGUAGES CXX)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/**.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(LIBRARIES "")
if (UNIX AND NOT APPLE)
  set(LIBRARIES ${LIBRARIES} X11 png GL)
elseif (WIN32)
  set(LIBRARIES ${LIBRARIES} user32 gdi32 opengl32 gdiplus Shlwapi dwmapi)
else ()
  message(FATAL_ERROR "Unsupported Platform")
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${LIBRARIES}
)
