cmake_minimum_required(VERSION 3.12.0)


project(ASCII-Boy)

set(SRC_DIR .${CURRENT_SOURCE_DIR}/src)
set(LIB_DIR .${CURRENT_SOURCE_DIR}/lib)

add_executable(
	${PROJECT_NAME}
	${SRC_DIR}/util/emath.cpp
	${SRC_DIR}/util/logger.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/app/program.cpp
	${SRC_DIR}/emu/gbstructs.cpp
	${SRC_DIR}/emu/gbsystem.cpp
	${SRC_DIR}/emu/cpu.cpp
	${SRC_DIR}/emu/mmu.cpp
	${SRC_DIR}/emu/cart.cpp
	)

find_package(Curses REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR} ${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

# -DCMAKE_BUILD_TYPE=* in CLI
# Release uses O3 and does not generate debug symbols
if(CMAKE_BUILD_TYPE STREQUAL "Release")

	target_compile_options(
		${PROJECT_NAME} PUBLIC
		-Wall
		-O3
		)

else() # Default is Debug

	target_compile_options(
		${PROJECT_NAME} PUBLIC
		-Wall
		-O0
		-g
	)

endif()

# ASCII-Boy makes use of C++17 features.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
