cmake_minimum_required(VERSION 2.8)
project(test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BUILD_TESTS TRUE)
set(BUILD_APPS TRUE)

macro(using name found dir lib)
  find_package(${name})
  if(NOT ${found})
    message(WARNING " ${name} not found!")
  else(NOT ${found})
    set(FOUND_PACKAGES ${FOUND_PACKAGES} ${name})
  endif(NOT ${found})
  set(DIR_TO_INCLUDE ${DIR_TO_INCLUDE} ${${dir}})
  set(LIB_TO_LINK ${LIB_TO_LINK} ${${lib}})
  message(STATUS ${${lib}})
endmacro(using)

#using(OpenGL  OPENGL_FOUND  OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
#using(GLUT    GLUT_FOUND    GLUT_INCLUDE_DIR   GLUT_LIBRARIES)
#using(Eigen3  EIGEN3_FOUND  EIGEN3_INCLUDE_DIR EIGEN3_LIBRARIES)
using(GTest   GTEST_FOUND   GTEST_INCLUDE_DIR  GTEST_LIBRARIES)
using(Threads Threads_FOUND ""                 CMAKE_THREAD_LIBS_INIT)
using(OpenCV2  OpenCV2_FOUND  OpenCV2_INCLUDE_DIRS OpenCV2_LIBRARIES)

include_directories(${DIR_TO_INCLUDE})

macro(expect_packages appname)
  set(list_var_temp "${ARGN}")  
  foreach(expect IN LISTS list_var_temp)
    string(FIND "${FOUND_PACKAGES}" "${expect}" EXPECT_FOUND)
    if(${EXPECT_FOUND} EQUAL -1)
      message(ERROR " Unable to build ${appname} because package ${expect} not found!")
    endif(${EXPECT_FOUND} EQUAL -1)
  endforeach(expect)
  unset(list_var_temp)
endmacro(expect_packages)

file(GLOB header "*.h")
file(GLOB src "*.cpp" "*.c" ".S")
add_library(maslab SHARED ${src} ${header})
target_link_libraries(maslab ${LIB_TO_LINK})
set_target_properties(maslab PROPERTIES LINKER_LANGUAGE CXX)
#add_library(maslab_static STATIC ${src} ${header})

if(${BUILD_TESTS})
  expect_packages(tests GTest)
  file(GLOB tests "test/*.cpp")
  foreach(test ${tests})
    get_filename_component(name ${test} NAME_WE)
    add_executable(${name} ${test})
    target_link_libraries(${name} maslab ${LIB_TO_LINK})
    #add_custom_command(TARGET ${name} POST_BUILD COMMAND "${CMAKE_BINARY_DIR}/bin/${name}")
  endforeach(test)
endif(${BUILD_TESTS})

if(${BUILD_APPS})
  add_subdirectory("app/")
endif(${BUILD_APPS})


