# Written in 2009 by Lloyd Hilaiel
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

set(SRCS elzma.c util.c)
set(HDRS util.h)

add_executable(elzma ${SRCS} ${HDRS})
target_link_libraries(elzma easylzma)

# make a hard link (or copy on win32) from unelzma to elzma
# get_target_property(binPath elzma LOCATION)
# if(WIN32)
#   set(exeExt ".exe")
#   set(hardLinkCommand ${CMAKE_COMMAND} -E copy_if_different)
# else()
#   set(hardLinkCommand ln -f)
#   set(exeExt)
# endif()
# add_custom_command(
#   TARGET elzma
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${binPath} ${binDir}
#   COMMAND ${hardLinkCommand} elzma${exeExt} unelzma${exeExt}
#   WORKING_DIRECTORY "${binDir}"
# )
