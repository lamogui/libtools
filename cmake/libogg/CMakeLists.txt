
cmake_minimum_required(VERSION 3.1.3)

file(GLOB src_ogg "src/*")
file(GLOB h_ogg "include/*")

include_directories(
  include/
)

add_library( 
  ogg
  STATIC 
  ${src_ogg}
  ${h_ogg}
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

message(STATUS "Install location: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

INSTALL(TARGETS ogg DESTINATION lib/)
install(DIRECTORY include/ogg/ DESTINATION include/ogg/)

if (MSVC) 
  foreach (MODE "_DEBUG" "_MINSIZEREL" "_RELEASE" "_RELWITHDEBINFO")
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_C_FLAGS${MODE}}")
            set(CMAKE_C_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
            string(REPLACE "/MD" "/MT" TMP "${CMAKE_CXX_FLAGS${MODE}}")
            set(CMAKE_CXX_FLAGS${MODE} "${TMP}" CACHE STRING "" FORCE)
  endforeach (MODE) 
endif()



  
  
  