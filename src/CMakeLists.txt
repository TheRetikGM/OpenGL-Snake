set(SRC_DIR ${CMAKE_SOURCE_DIR})
configure_file(config.h.in config.h @ONLY)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src/include
	${CMAKE_SOURCE_DIR}/include
)

# Main executable
file(GLOB SHADERS
	"shaders/*.frag"
	"shaders/*.vert"
	"shaders/*.geom"
)

file(GLOB base_files "./*.cpp")
file(GLOB base_includes 
	"include/*.h" 
	"include/*.hpp" 
	"include/interfaces/*"
)

add_executable(${PROJECT_NAME}
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	${base_files} ${base_includes}
	${SHADERS}
)
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "" FILES ${SHADERS})

source_group(TREE ${CMAKE_SOURCE_DIR}/src/include PREFIX "Header Files/Renderers" FILES include/tilemap_renderer.h include/sprite_renderer.h include/basic_renderer.h)
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "Source Files/Renderers" FILES tilemap_renderer.cpp sprite_renderer.cpp basic_renderer.cpp)
