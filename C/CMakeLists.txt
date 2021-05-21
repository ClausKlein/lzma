# https://www.7-zip.org/sdk.html
# usage:
#$ cmake -B build -S .
#$ DESTDIR=/tmp/lzma make -C build install
#

option(CMAKE_EXPORT_COMPILE_COMMANDS "for clang-tidy" ON)

cmake_minimum_required(VERSION 3.13...3.20)

# use ccache if found
find_program(CCACHE_EXECUTABLE "ccache" HINTS /usr/local/bin /opt/local/bin)
if(CCACHE_EXECUTABLE AND NOT CMAKE_TOOLCHAIN_FILE)
  message(STATUS "use ccache")
  set(CMAKE_C_COMPILER_LAUNCHER
      "${CCACHE_EXECUTABLE}"
      CACHE PATH "ccache" FORCE
  )
endif()

# ==============================
project(lzma LANGUAGES C)
# ==============================

# Where to put all the RUNTIME targets when built.  This variable is used to
# initialize the RUNTIME_OUTPUT_DIRECTORY property on all the targets.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
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
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-overlength-strings)
  endif()
endif()

# ---------------------------------------------------------------------------------------
#XXX find_package(Threads REQUIRED)

add_library(
  lzma_s STATIC
  # TBD added for test! CK
  7zCrcOpt.c
  7zCrc.c
  7zCrc.h
  CpuArch.c
  ####################
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
)
target_compile_definitions(lzma_s PUBLIC _7ZIP_ST)
target_include_directories(lzma_s PUBLIC ${PROJECT_SOURCE_DIR})
#XXX target_link_libraries(lzma_s Threads::Threads)

# use the lib to build bin
add_executable(lzma Util/Lzma/LzmaUtil.c)
target_link_libraries(lzma lzma_s)

# ---------------------------------------------------------------------------------------
# test support
enable_testing()
add_subdirectory(test)

add_test(NAME test_encode_bin COMMAND lzma e $<TARGET_FILE:lzma> data.lzma)
add_test(NAME test_decode_bin COMMAND lzma d data.lzma data.bin)
add_test(NAME test_check_bin COMMAND cmake -E compare_files $<TARGET_FILE:lzma> data.bin)
add_test(NAME test_encode_txt COMMAND lzma e ${PROJECT_SOURCE_DIR}/../README.rst README.lzma)
add_test(NAME test_decode_txt COMMAND lzma d README.lzma README.rst)
add_test(NAME test_check COMMAND cmake -E compare_files ${PROJECT_SOURCE_DIR}/../README.rst README.rst)
add_test(NAME test_lzma_cat COMMAND lzma d README.lzma)
add_test(NAME test_lzma_err COMMAND lzma e README.lzma)
# add_test(NAME test_write_err COMMAND lzma d README.lzma /var/tmp/NO/dir || echo OK)
# add_test(NAME test_read_err COMMAND lzma d NO.lzma || echo OK)
add_test(NAME test_help COMMAND lzma)

# ---------------------------------------------------------------------------------------
# install the bin
install(TARGETS lzma DESTINATION bin)

# ---------------------------------------------------------------------------------------
option(USE_CPPCHECK "Looking for cppcheck program ..." ON)
if(USE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    find_program(HTMLREPORT cppcheck-htmlreport)
    if(HTMLREPORT)
      message(STATUS "cppchek found at '${CPPCHECK}'; you may use target 'cppcheck' to run it!")
      add_custom_target(
        cppcheck
        ${CPPCHECK}
        --platform=unix32
        --std=posix
        --verbose
        --force
        --enable=all
        --inconclusive
        --template=gcc
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
endif()

# ---------------------------------------------------------------------------------------
if("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")
  include(cmake/CodeCoverage.cmake)

  setup_target_for_coverage_lcov(
    NAME
    lcov # New target name
    EXECUTABLE
    ctest
    DEPENDENCIES
    all # build this targets first
    EXCLUDE
    /Applications/*
    /usr/*
    NO_DEMANGLE
  )
endif()

# ---------------------------------------------------------------------------------------
include(CMakePrintSystemInformation)
# ---------------------------------------------------------------------------------------
