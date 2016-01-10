cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")
file(GLOB header "${CMAKE_CURRENT_SOURCE_DIR}/../*.h")

file(GLOB apps "*.cpp")

foreach(app ${apps})
  get_filename_component(name ${app} NAME_WE)
  add_executable(${name} ${app} ${header})
  target_link_libraries(${name} maslab)
endforeach(app)
