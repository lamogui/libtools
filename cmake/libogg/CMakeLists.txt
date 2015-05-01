
cmake_minimum_required(VERSION 3.1.3)

file(GLOB src_ogg "src/*")
file(GLOB h_ogg "include/*")

# define a macro that helps defining an option
macro(libtools_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

libtools_set_option(LIB_INSTALL_DIR lib STRING "The lib destination dir (lib, lib32 or lib64)")

if ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
  MESSAGE( STATUS "Build target is 64Bits" )
elseif( "${CMAKE_SIZEOF_VOID_P}" EQUAL "4" )
  MESSAGE( STATUS "Build target is 32Bits" )
endif()

include_directories(
  include/
)

add_library( 
  ogg
  STATIC 
  ${src_ogg}
  ${h_ogg}
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${LIB_INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${LIB_INSTALL_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

message(STATUS "Install location: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

INSTALL(TARGETS ogg DESTINATION ${LIB_INSTALL_DIR}/)
install(DIRECTORY include/ogg/ DESTINATION include/ogg/)

if (MSVC) 
  foreach (MODE "_DEBUG" "_MINSIZEREL" "_RELEASE" "_RELWITHDEBINFO")
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_C_FLAGS${MODE}}")
            set(CMAKE_C_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_CXX_FLAGS${MODE}}")
            set(CMAKE_CXX_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
  endforeach (MODE) 
endif()



  
  
  