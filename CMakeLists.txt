cmake_minimum_required(VERSION 3.13.4)

project("main")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX "d")

add_subdirectory(external/json-develop)
add_subdirectory(external/engine)

if (UNIX)
	#add_compile_options(-O1 -g -fsanitize=address -fno-omit-frame-pointer)
	add_compile_options(-Wall -Wno-reorder)
endif()
add_subdirectory("src")

target_link_directories(${PROJECT_NAME} PRIVATE lib)
target_include_directories(${PROJECT_NAME} 
	PRIVATE include 
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
	PRIVATE external/Physics2D/include
	PRIVATE external/engine/include
	PRIVATE external/engine/imgui
	PRIVATE external/engine/imgui/backends
)
target_link_libraries(${PROJECT_NAME}
	${ENGINE_LINK_LIBS}
)
target_link_libraries(${PROJECT_NAME} 
	nlohmann_json::nlohmann_json
)

# Install defines
if (MSVC)
	include(CPack)
endif()
install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION ./)
if (EXISTS "${CMAKE_SOURCE_DIR}/src/shaders")
	install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/shaders" DESTINATION ./)
endif()
