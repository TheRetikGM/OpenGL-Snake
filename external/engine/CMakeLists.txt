cmake_minimum_required(VERSION 3.13.0)

project("Engine")
set(ENGINE_NAME "Engine")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX "d")
set(ENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ENGINE_LINK_LIBS ${ENGINE_NAME} PARENT_SCOPE)
configure_file(engine_config.h.in engine_config.h @ONLY)

file(GLOB engine_src_files "*.cpp")
file(GLOB engine_include_files 
	"include/*.h" 
	"include/*.hpp" 
	"include/interfaces/*.h"
	"include/engine/*"
	"include/engine/interface/*"
)

add_library(Glad glad.c include/glad/glad.h)
target_include_directories(Glad PRIVATE include)

add_library(${ENGINE_NAME}
	${engine_src_files} ${engine_include_files}
	${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
)
target_link_directories( ${ENGINE_NAME} PRIVATE lib )
target_include_directories(${ENGINE_NAME}
	PRIVATE "${CMAKE_SOURCE_DIR}/include"
	PRIVATE include
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(${ENGINE_NAME} Glad)

if (MSVC)
	target_link_libraries(${ENGINE_NAME}
		opengl32
		debug Debug/glfw3d optimized Release/glfw3
		debug Debug/freetyped optimized Release/freetype
	)
elseif (UNIX)
    target_link_libraries(${ENGINE_NAME}
        glfw
        GL
        X11
        pthread
        Xrandr
        Xi
        dl
        m
        z
		freetype
    )
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${engine_include_files})


# Dear ImGui
set(IMGUI_DIR imgui)
set(IMGUI ImGui)
file(GLOB imgui_sources 
	"${IMGUI_DIR}/*.cpp" 
	"${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
	"${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
)
file(GLOB imgui_headers 
	"${IMGUI_DIR}/*.h" 
	"${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
	"${IMGUI_DIR}/backends/imgui_impl_glfw.h"
)
add_library(${IMGUI} ${imgui_sources} ${imgui_headers})
target_include_directories(${IMGUI} PUBLIC ${IMGUI_DIR} "${IMGUI_DIR}/backends" include)

# Link Dear ImGui
target_link_libraries(${ENGINE_NAME} ${IMGUI})

# === Install ===
install(DIRECTORY "${ENGINE_SRC_DIR}/shaders" DESTINATION ./)
