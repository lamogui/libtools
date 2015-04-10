
cmake_minimum_required(VERSION 3.1.3)


set(libvorbis_SRCS
  lib/analysis.c
  lib/bitrate.c
  lib/block.c
  lib/codebook.c
  lib/envelope.c
  lib/floor0.c
  lib/floor1.c
  lib/info.c
  lib/lookup.c
  lib/lpc.c
  lib/lsp.c
  lib/mapping0.c
  lib/mdct.c
  lib/psy.c
  lib/registry.c
  lib/res0.c
  lib/sharedbook.c
  lib/smallft.c
  lib/synthesis.c
  lib/vorbisenc.c
  lib/window.c
)

set(libvorbis_HDRS
  lib/backends.h
  lib/bitrate.h
  lib/codebook.h
  lib/codec_internal.h
  lib/envelope.h
  lib/modes/floor_all.h
  lib/books/floor/floor_books.h
  lib/highlevel.h
  lib/lookup.h
  lib/lookup_data.h
  lib/lpc.h
  lib/lsp.h
  lib/masking.h
  lib/mdct.h
  lib/misc.h
  lib/os.h
  lib/psy.h
  lib/modes/psych_11.h
  lib/modes/psych_16.h
  lib/modes/psych_44.h
  lib/modes/psych_8.h
  lib/registry.h
  lib/books/coupled/res_books_stereo.h
  lib/books/uncoupled/res_books_uncoupled.h
  lib/modes/residue_16.h
  lib/modes/residue_44.h
  lib/modes/residue_44u.h
  lib/modes/residue_8.h
  lib/scales.h
  lib/modes/setup_11.h
  lib/modes/setup_16.h
  lib/modes/setup_22.h
  lib/modes/setup_32.h
  lib/modes/setup_44.h
  lib/modes/setup_44u.h
  lib/modes/setup_8.h
  lib/modes/setup_X.h
  lib/smallft.h
  lib/window.h
)

set(
  vorbis_public_HDRS
  include/vorbis/codec.h
  include/vorbis/vorbisenc.h
  include/vorbis/vorbisfile.h
)

IF(MSVC)
add_definitions(/D_UNICODE /DUNICODE)
list(APPEND libvorbis_SRCS win32/vorbis.def)
ENDIF(MSVC)





include_directories(
  include/
  lib/
)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


add_library(
  vorbis 
  STATIC
  ${libvorbis_SRCS} 
  ${libvorbis_HDRS} 
  ${vorbis_public_HDRS}
)

add_library(
  vorbisfile 
  STATIC
  lib/vorbisfile.c 
  include/vorbis/vorbisfile.h
)




message(STATUS "Install location: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

install(
  TARGETS
  vorbis
  vorbisfile
  DESTINATION lib/
)
install(DIRECTORY include/vorbis/ DESTINATION include/vorbis/)


if (MSVC) 
  foreach (MODE "_DEBUG" "_MINSIZEREL" "_RELEASE" "_RELWITHDEBINFO")
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_C_FLAGS${MODE}}")
            set(CMAKE_C_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_CXX_FLAGS${MODE}}")
            set(CMAKE_CXX_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
  endforeach (MODE) 
endif()
  
  
  