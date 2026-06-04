cmake_minimum_required(VERSION 3.28)
project(Cube)

set(exec Cube)
set(src
	src/main.cpp
)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${exec} ${src})

target_include_directories(${exec} PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

if(UNIX)
	target_include_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/include
	)
	target_link_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/lib
	)
	target_link_libraries(${exec} PUBLIC
		raylib
		X11
	)
elseif(WIN32)
	target_include_directories(${exec} 
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/include
	)
	target_link_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/lib
	)
	target_link_libraries(${exec} PUBLIC
		raylib
		gdi32
		winmm
	)
endif()
