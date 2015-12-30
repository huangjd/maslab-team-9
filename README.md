#################################################################################
Copyright (C) 2015-2016 - 2016 MIT Mobile Autonomous Systems Lab (6.146) Team 9
  William Junda Huang '16 <huangjd@mit.edu>
  Mengyuan Sun '19 <mysun@mit.edu>
  Alan D. Cheng '19 <adcheng@mit.edu>
  Tugsbayasgalan Manlaibaatar '19 <tugsuu@mit.edu>
  Kristina Elizabeth Schmidt '17 <schmidtk@mit.edu>

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
  


