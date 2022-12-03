cmake_minimum_required(VERSION 3.12.0)


project(ASCII-Boy)

set(SRC_DIR .${CURRENT_SOURCE_DIR}/src)

add_executable(
	${PROJECT_NAME}
	${SRC_DIR}/main.cpp
)

# ASCII-Boy makes use of C++20 features.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
