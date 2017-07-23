# GPhoto2 and CMAKE Basic Example

Credit for the following files goes to the following authors/creators:

1. SRC C Main Program - Pablo Roman http://sepharads.blogspot.com.au/2011/11/camera-tethered-capturing-using.html
2. FindGphoto2.cmake - darktable.org  https://www.darktable.org/
3. LibFindMacros.cmake -  darktable.org  https://www.darktable.org/

## To Install GPhoto2 (Linux)

1. Install gphoto2 sudo apt-get install gphoto2
2. May need to install sudo apt-get install libgphoto2-dev
3. May need to install sudo apt-get install libgphoto2-6

## How To Run the Example

1. Download the Repository
2. Open a terminal and cd to the example directory of the repository.
3. Run cmake `cmake .` to generate the makefiles.
4. Run make `make` to build the program.
5. Run the program using './bin/main'

## How To CMake Your Own Source Code
Assuming you are using cmake to compile your source code you can include libgphoto2 by doing the following

1. Copy the cmake_modules folder into your own project directory.
2. In your CMakeLists.txt add the following lines of code (in the appropriate places, see the CMakeLists.txt in the example)
	* `list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")`
	* `find_package(Gphoto2 REQUIRED)`
	* `target_link_libraries(<PROJECT_NAME> ${Gphoto2_LIBRARIES})`