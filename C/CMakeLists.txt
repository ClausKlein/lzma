# https://www.7-zip.org/sdk.html
# usage:
#$ cmake -B build -S .
#$ DESTDIR=/tmp/lzma make -C build install
#

option(CMAKE_EXPORT_COMPILE_COMMANDS "for clang-tidy" ON)

cmake_minimum_required(VERSION 3.13...3.20)

## use ccache if found
find_program(CCACHE_EXECUTABLE "ccache" HINTS /usr/local/bin /opt/local/bin)
if(CCACHE_EXECUTABLE AND NOT CMAKE_TOOLCHAIN_FILE)
  message(STATUS "use ccache")
  set(CMAKE_C_COMPILER_LAUNCHER
      "${CCACHE_EXECUTABLE}"
      CACHE PATH "ccache" FORCE
  )
endif()

#========================
project(lzma LANGUAGES C)
#========================

message(STATUS "CMAKE_C_PLATFORM_ID=${CMAKE_C_PLATFORM_ID}")

#---------------------------------------------------------------------------------------
# Compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)

option(COMPILER_WARNINGS_ARE_ERRORS "To be pedantic! ;-)" ON)
if(COMPILER_WARNINGS_ARE_ERRORS)
  if(MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
  else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
  endif()
endif()

#XXX find_package(Threads REQUIRED)

#
# Where to put all the RUNTIME targets when built.  This variable is used to
# initialize the RUNTIME_OUTPUT_DIRECTORY property on all the targets.
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# use the lib to build bin
add_executable( lzma
    7zFile.c
    7zFile.h
    7zStream.c
    7zTypes.h
    7zVersion.h
    Alloc.c
    Alloc.h
    Compiler.h
    CpuArch.h
    LzFind.c
    LzFind.h
    LzHash.h
    LzmaDec.c
    LzmaDec.h
    LzmaEnc.c
    LzmaEnc.h
    Precomp.h
    Util/Lzma/LzmaUtil.c
)
target_compile_definitions(lzma PUBLIC _7ZIP_ST)
#XXX target_link_libraries(lzma Threads::Threads)

# install the bin
install(TARGETS lzma DESTINATION bin)

option(USE_CPPCHECK "Looking for cppcheck program ..." ON)
if(USE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  find_program(HTMLREPORT cppcheck-htmlreport)
  if(CPPCHECK AND HTMLREPORT)
    message(STATUS "cppcheck found at '${CPPCHECK}'; you may use target 'cppcheck' to run it!")
    add_custom_target(
      cppcheck
      ${CPPCHECK}
      --std=c99
      --std=posix
      --verbose
      --force
      --enable=all
      --inconclusive
      --template=gcc
      --suppress=variableScope
      --xml
      --xml-version=2
      .
      2>
      cppcheck.xml
      COMMAND ${HTMLREPORT} --file=cppcheck.xml --report-dir=cppcheck
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      VERBATIM
    )
  endif()
endif()

include(CMakePrintSystemInformation)
message("CMAKE_C_LIBRARY_ARCHITECTURE ${CMAKE_C_LIBRARY_ARCHITECTURE}")
