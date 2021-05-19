# Written in 2009 by Lloyd Hilaiel
#
# License
#
# All the cruft you find here is public domain.  You don't have to credit
# anyone to use this code, but my personal request is that you mention
# Igor Pavlov for his hard, high quality work.
#

add_library(easylzma_s STATIC)

if(WIN32)
  add_definitions(-DWIN32)
endif()

# file(GLOB SRCS easylzma/src/*.c)
# file(GLOB HDRS easylzma/src/*.h)
# file(GLOB PUB_HDRS easylzma/src/easylzma/*.h)
# 
# target_sources(easylzma_s PRIVATE ${SRCS} ${HDRS} PUBLIC ${PUB_HDRS})
# target_include_directories(easylzma_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
# target_link_libraries(easylzma_s PUBLIC lzma_s)

add_subdirectory(easylzma)

set(SRCS easylzma_test.c simple.c)
set(HDRS simple.h)

add_executable(easylzma_test ${SRCS} ${HDRS})
target_include_directories(easylzma_test PRIVATE easylzma/src)
target_link_libraries(easylzma_test easylzma_s)

add_test(NAME easylzma_test COMMAND easylzma_test)
