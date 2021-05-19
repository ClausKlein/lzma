#
# License
#
# All the cruft you find here is public domain.  You don't have to credit
# anyone to use this code, but my personal request is that you mention
# Igor Pavlov for his hard, high quality work.
#

if(WIN32)
  add_definitions(-DWIN32)
endif()

file(GLOB SRCS     src/*.c)
file(GLOB HDRS     src/*.h)
file(GLOB PUB_HDRS src/easylzma/*.h)
#
# TODO install public headers! CK
#

target_sources(easylzma_s PRIVATE ${SRCS} ${HDRS} PUBLIC ${PUB_HDRS})
target_include_directories(easylzma_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(easylzma_s PUBLIC lzma_s)
