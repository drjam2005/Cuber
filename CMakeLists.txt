cmake_minimum_required(VERSION 3.28)
project(Cube)

set(exec Cube)
set(src
	src/main.cpp
	src/helpers.cpp
	src/game.cpp
	src/game_misc.cpp
	src/game_ui.cpp
	src/game_scores.cpp
	src/cube/cube.cpp
	src/cube/cube_animation.cpp
	src/cube/cube_renderer.cpp
	src/cube/cube_updater.cpp
	src/cube/cube_misc.cpp
	src/cube/cube_moves.cpp
	src/cube/cube_scrambler.cpp
	src/standard_pieces.cpp
)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${exec} ${src})
add_subdirectory(vendor/mINI)

target_include_directories(${exec} PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${exec} PUBLIC
	mini
)

target_compile_options(${exec} PUBLIC
	-O3
)

if(UNIX)
	target_include_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/include
	)
	target_link_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/lib
	)
	target_link_libraries(${exec} PUBLIC
		libraylib.a
		X11
	)
elseif(WIN32)
	target_include_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/include
	)
	target_link_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/lib
	)
	target_link_libraries(${exec} PUBLIC
		libraylib.a
		gdi32
		winmm
	)
	target_link_options(${exec} PUBLIC
		-mwindows
	)
endif()
