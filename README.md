#################################################################################
Copyright (C) 2015-2016 - 2016 MIT Mobile Autonomous Systems Lab (6.146) Team 9
  William Junda Huang '16 <huangjd@mit.edu>
  Mengyuan Sun '19 <mysun@mit.edu>
  Alan De-Hao Cheng '19 <adcheng@mit.edu>
  Tugsbayasgalan Manlaibaatar '19 <tugsuu@mit.edu>

#################################################################################

doc/    Documents, including any official notes

cad/    Mechanical design goes here

src/    Main source code, for everything go into the tablet. All source files in this folder (not sub-folders) are linked against each other, so don't put main.cpp here.
  app/  Applications. Each independent executable or library should have its own sub-folder in this folder (for example, app/Simulator/), and it will be linked against all shared files in app/.
  test/ Automated tests using GTest. Run by default build.
 
build/  Source code will be built here, and all the contents in the folder are automatically generated and are platform dependent. DO NOT commit anything in this folder.
 
CMakeModules/ Custom CMake modules for some libraries.


To build the source code, type the following commands in the project directory:

  cd build/
  cmake ..
  make
  

<img src="https://cloud.githubusercontent.com/assets/4660595/22714417/6224cdda-ed5a-11e6-87d0-933745653ffd.PNG" width="200" title="3d view of the mechanical assembly" >

<img src="https://cloud.githubusercontent.com/assets/4660595/22714416/6220cf28-ed5a-11e6-8957-8a8b797beca6.PNG" width="200" title="Front view of the mechanical assembly" >

<img src="https://cloud.githubusercontent.com/assets/4660595/22714415/6220b4c0-ed5a-11e6-8230-7574ae27a7da.PNG" width="200" title="Side view of the mechanical assembly" >

