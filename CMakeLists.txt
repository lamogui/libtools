
cmake_minimum_required(VERSION 3.1.3)
project(libtools)


# define a macro that helps defining an option
macro(libtools_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

libtools_set_option(LIB_INSTALL_DIR lib STRING "The lib destination dir (lib, lib32 or lib64)")

# option(SFML_AUDIO_DRIVER "Build with SFML-AUDIO Driver support" OFF)

# if (SFML_AUDIO_DRIVER)
  # add_definitions(-DSFML_AUDIO_DRIVER)
# endif()

# SET(IMPLEMENTATION_TYPE "SFML 2.X" CACHE STRING "Library used for implentation")
# SET_PROPERTY(CACHE IMPLEMENTATION_TYPE PROPERTY STRINGS "SFML 2.X" "STL 2011") 

# if ($(IMPLEMENTATION_TYPE) MATCHES "SFML 2.X")
  # add_definitions(-DBUILD_LIBTOOLS_SFML)
# elseif ($(IMPLEMENTATION_TYPE) MATCHES "STL 2011")
  # add_definitions("-std=c++11")
# endif()

include_directories(
  include/
)

option(BUILD_SFML2_IMPLEMENTATION "Build with SFML 2.X support" ON)
option(BUILD_STL_2011_IMPLEMENTATION "Build with STL C++ 2011 support" ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${LIB_INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${LIB_INSTALL_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_definitions(-DSFML_STATIC)

#defines files 
#src
#src_sfml
file(GLOB src_sfml "src/sfml/*")
#src_core
file(GLOB src_core "src/core/*")
#src_public
file(GLOB src_public "src/public/*")
#src_drivers
file (GLOB src_drivers "src/drivers/*")
#src_decoders
file (GLOB src_decoders "src/decoders/*")
#src_shaders
file (GLOB src_shaders "src/shaders/*")
#h
#h_sfml
file(GLOB h_sfml "include/libtools/sfml/*")
#h_core
file(GLOB h_core "include/libtools/core/*")
#h_public
file(GLOB h_public "include/libtools/public/*")
#h_drivers
file (GLOB h_drivers "include/libtools/drivers/*")
#h_decoders
file (GLOB h_decoders "include/libtools/decoders/*")
#all includes
file (GLOB h_shaders "include/libtools/shaders/*")
#src groups
source_group("core" FILES  ${src_core} ${h_core})
source_group("drivers" FILES  ${src_drivers} ${h_drivers})
source_group("decoders" FILES  ${src_decoders} ${h_decoders})
source_group("sfml" FILES  ${src_sfml} ${h_sfml})
source_group("public" FILES  ${src_public} ${h_public})
source_group("shaders" FILES  ${src_shaders} ${h_shaders})

MESSAGE( STATUS "Install location: " ${CMAKE_INSTALL_PREFIX} )

if (BUILD_SFML2_IMPLEMENTATION)
  add_library( 
    tools-sfml
    STATIC 
    ${src_core}
    ${h_core}
    ${src_drivers}
    ${h_drivers}
    ${src_decoders}
    ${h_decoders}
    ${src_public}
    ${h_public}
    ${src_sfml}
    ${h_sfml}
    ${src_shaders}
    ${h_shaders}
  )
  target_compile_options( tools-sfml PRIVATE "-DBUILD_LIBTOOLS_SFML")
  set_target_properties( tools-sfml PROPERTIES LIBRARY_OUTPUT_NAME_DEBUG tools-sfml-d)
  set_target_properties( tools-sfml PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES ARCHIVE_OUTPUT_NAME_DEBUG tools-sfml-d)
  set_target_properties( tools-sfml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set_target_properties( tools-sfml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  INSTALL(TARGETS tools-sfml DESTINATION ${LIB_INSTALL_DIR}/)
endif()

if (BUILD_STL_2011_IMPLEMENTATION)
  add_library( 
    tools-c++11
    STATIC 
    ${src_core}
    ${h_core}
    ${src_drivers}
    ${h_drivers}
    ${src_decoders}
    ${h_decoders}
    ${src_public}
    ${h_public}
    ${src_shaders}
    ${h_shaders}
  )
  target_compile_options( tools-c++11 PRIVATE "-std=c++11")
  set_target_properties(tools-c++11 PROPERTIES LIBRARY_OUTPUT_NAME_DEBUG tools-c++11-d)
  set_target_properties(tools-c++11 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES ARCHIVE_OUTPUT_NAME_DEBUG tools-c++11-d)
  set_target_properties(tools-c++11 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set_target_properties(tools-c++11 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  INSTALL(TARGETS tools-sfml DESTINATION ${LIB_INSTALL_DIR}/)
endif()

if (MSVC) 
  foreach (MODE "_DEBUG" "_MINSIZEREL" "_RELEASE" "_RELWITHDEBINFO")
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_C_FLAGS${MODE}}")
            set(CMAKE_C_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_CXX_FLAGS${MODE}}")
            set(CMAKE_CXX_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
  endforeach (MODE) 
endif()

INSTALL(DIRECTORY include/ DESTINATION include)
