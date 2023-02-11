cmake_minimum_required(VERSION 3.2)

project(BinTranslator
		VERSION 1.0
		LANGUAGES CXX)

if(NOT DEFINED BUILD_MODE)
	message(FATAL_ERROR "THe build mode wasn't specified.")
endif()

set(BIN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/bin/${BUILD_MODE}")
set(LIB_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/lib/${BUILD_MODE}")

if(BUILD_MODE STREQUAL "Debug")
	add_compile_options(-Og -Wall)

elseif(BUILD_MODE STREQUAL "Release")
	add_compile_options(-O3 -Wall)

endif()

if(DEFINED CONFIGURE_ST AND CONFIGURE_ST)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_compile_options(--verbose -target x86_64-pc-windows-gnu --sysroot=C:/Development/MinGW)

endif()

add_executable(prog
			src/main.cpp
			src/BinaryTranslator.cpp
			)

target_compile_features(prog PUBLIC cxx_std_17)
target_include_directories(prog PUBLIC "include")

set_target_properties( prog
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
)
