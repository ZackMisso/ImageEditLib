cmake_minimum_required(VERSION 3.1)

project(imedit)

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Quench annoying deprecation warnings when compiling GLFW on OSX
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|Intel)$")
  CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

  if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "Unsupported compiler -- pybind11 requires C++11 support!")
  endif()
endif()

if (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(opengl_library OpenGL)
  find_library(corevideo_library CoreVideo)
  find_library(iokit_library IOKit)
  list(APPEND NANOGUI_EXTRA_LIBS ${cocoa_library} ${opengl_library} ${corevideo_library} ${iokit_library})
  list(APPEND LIBNANOGUI_EXTRA_SOURCE src/darwin.mm)
 endif()

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# include directories
include_directories(# Include directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/
                    # STB library for image io
                    ${PROJECT_SOURCE_DIR}/ext/stb/
                    ext/
)

set(ext_stb_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h
)

add_definitions(-DDATA_DIR=R\"\(${PROJECT_SOURCE_DIR}/data\)\")

set(imedit_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    include/imedit/image.h
    include/imedit/procedural.h

    test/main.cpp
)

set(texture_gen_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    include/imedit/image.h
    include/imedit/procedural.h
    include/imedit/filter.h

    test/texture_gen.cpp
)

add_executable(imedit ${imedit_srcs})
add_executable(texture_gen ${texture_gen_srcs})

target_link_libraries(imedit ${PYTHON_LIBRARIES})
target_link_libraries(texture_gen ${PYTHON_LIBRARIES})

set(CompilerFlags
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELWITHDEBINFO COMPILE_DEFINITIONS U_CMAKE_BUILD_TYPE
      CMAKE_MACOSX_RPATH
)

# Create the output data directory
# add_custom_command(TARGET imedit POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/data/output") # a5
