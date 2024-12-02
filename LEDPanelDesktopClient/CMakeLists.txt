﻿# CMakeList.txt : CMake project for LEDPanelDesktopClient, include source and define
# project specific logic here.
#

set(SDL3_STATIC OFF)

find_package(SDL3 REQUIRED)

add_library("ImGui" SHARED ./ThirdParties/imgui/imgui.cpp ./ThirdParties/imgui/imgui_draw.cpp
./ThirdParties/imgui/imgui_demo.cpp ./ThirdParties/imgui/imgui_tables.cpp ./ThirdParties/imgui/imgui_widgets.cpp 
./ThirdParties/imgui/backends/imgui_impl_sdl3.cpp ./ThirdParties/imgui/backends/imgui_impl_sdlrenderer3.cpp)
if (SDL3_FOUND)
	target_link_libraries(ImGui PUBLIC ${SDL3_LIBRARIES})
else()
	message(FATAL_ERROR "Cannot find SDL3")
endif()
find_package(OpenCV REQUIRED)

# Add source to this project's executable.
add_executable (LEDPanelDesktopClient "main.cpp")

#target_link_libraries(LEDPanelDesktopClient PRIVATE SDL3::SDL3-shared)
target_link_libraries(LEDPanelDesktopClient PRIVATE ImGui)
target_link_libraries(LEDPanelDesktopClient PRIVATE ${OpenCV_LIBS})
target_link_libraries(LEDPanelDesktopClient PUBLIC ${SDL3_LIBRARIES})
if(WIN32)
	target_link_libraries(LEDPanelDesktopClient PRIVATE ws2_32)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(./ThirdParties/imgui)
include_directories(./ThirdParties/asio/asio/include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LEDPanelDesktopClient PROPERTY CXX_STANDARD 20)
endif()
# TODO: Add tests and install targets if needed.
