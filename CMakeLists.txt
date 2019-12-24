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
                    # PCG library for random numbers
                    ${PROJECT_SOURCE_DIR}/ext/pcg/
                    # TinyEXR library for EXR support
                    ${PROJECT_SOURCE_DIR}/ext/tinyexr/
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

    ext/pcg/pcg32.h

    ext/tinyexr/tinyexr.h

    include/imedit/image.h
    include/imedit/procedural.h

    src/image.cpp

    test/main.cpp
)

set(texture_gen_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    ext/pcg/pcg32.h

    ext/tinyexr/tinyexr.h

    include/imedit/image.h
    include/imedit/procedural.h
    include/imedit/filter.h

    src/image.cpp

    test/texture_gen.cpp
)

set(move_demo_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    ext/pcg/pcg32.h

    ext/tinyexr/tinyexr.h

    include/imedit/image.h
    include/imedit/procedural.h
    include/imedit/filter.h

    src/image.cpp

    test/move_demo.cpp
)

set(adrien_name_demo_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    ext/pcg/pcg32.h

    ext/tinyexr/tinyexr.h

    include/imedit/image.h
    include/imedit/procedural.h
    include/imedit/filter.h

    src/image.cpp

    test/adrien_name_demo.cpp
)

set(displacement_demo_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    ext/pcg/pcg32.h

    ext/tinyexr/tinyexr.h

    include/imedit/image.h
    include/imedit/procedural.h
    include/imedit/filter.h

    src/image.cpp

    test/displacement_demo.cpp
)

set(color_map_srcs
    ext/stb/stb_image_write.h
    ext/stb/stb_image.h

    ext/pcg/pcg32.h

    ext/tinyexr/tinyexr.h

    include/imedit/image.h
    include/imedit/procedural.h
    include/imedit/filter.h

    src/image.cpp

    test/color_maps.cpp
)

add_executable(imedit ${imedit_srcs})
add_executable(texture_gen ${texture_gen_srcs})
add_executable(move_demo ${move_demo_srcs})
add_executable(adrien_name_demo ${adrien_name_demo_srcs})
add_executable(displacement_demo ${displacement_demo_srcs})
add_executable(color_maps ${color_map_srcs})

target_link_libraries(imedit ${PYTHON_LIBRARIES})
target_link_libraries(texture_gen ${PYTHON_LIBRARIES})
target_link_libraries(move_demo ${PYTHON_LIBRARIES})
target_link_libraries(adrien_name_demo ${PYTHON_LIBRARIES})
target_link_libraries(displacement_demo ${PYTHON_LIBRARIES})
target_link_libraries(color_maps ${PYTHON_LIBRARIES})

set(CompilerFlags
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELWITHDEBINFO COMPILE_DEFINITIONS U_CMAKE_BUILD_TYPE
      CMAKE_MACOSX_RPATH
)
