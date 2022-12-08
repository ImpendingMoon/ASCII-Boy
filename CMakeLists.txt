cmake_minimum_required(VERSION 3.12.0)


project(ASCII-Boy)

set(SRC_DIR .${CURRENT_SOURCE_DIR}/src)

add_executable(
	${PROJECT_NAME}
	${SRC_DIR}/main.cpp
	${SRC_DIR}/util/emath.cpp
	${SRC_DIR}/util/logger.cpp
	${SRC_DIR}/emu/gbstructs.cpp
	${SRC_DIR}/emu/gbsystem.cpp
	${SRC_DIR}/emu/cpu.cpp
	${SRC_DIR}/emu/mmu.cpp
)

target_link_libraries(${PROJECT_NAME} fmt)

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -g)
# ASCII-Boy makes use of C++17 features.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
