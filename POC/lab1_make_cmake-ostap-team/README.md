# Lab work <mark>1</mark>: <mark>WRITE MAKE AND CMAKE FILES FOR LIB COMPILATION</mark>
- Authors: [Ostap Trush](https://github.com/Adeon18)
- Variant: 3(zlib). **WITH FIXES!**
### PREREQUIREMENTS 

- Cmake and make must be installed on your system.
- Also a compiler - GCC or clang.
- A working OS(Preferably Linux, but the entire thing should work for Windows as well.)

### Usage

Clone the repository
```
$ git clone https://github.com/ucu-cs/lab1_make_cmake-ostap-team.git
$ cd lab1_make_cmake-ostap-team
```
Use Makefiles
```
$ cd src/library
$ make
*some output*
$ cd bin
$ ls
libzlib.a libzlib.so
$ cd ../../example
$ make
*Set the absolute library path to what was generated in library/bin. Here is the example for Linux(My system)*
$ export LD_LIBRARY_PATH=/home/adeon/Desktop/coding/c_language/labs/lab0/lab1_make_cmake-ostap-team/src/library/bin
$ cd bin
$ ls
example minigzip
```
Now feel free to run those files and they should work from any directory as long as LD_LIBRARY_PATH is **absolute** and correctly set.

Now Cmake files. Clean all the tmp files by going in each dir and typing ```make clean```.
```
$ cd src/library
$ mkdir build
$ cmake ..
*some output*
$ make
$ cd ../../example
$ mkdir build
$ cmake ..
*some output*
$ make
```

All your compiled results will be lying in build directory of relative folders. 

**!!!ATTENTION!!!** The build folder for the library must be named "build".

### Important!

The usage examples above may have some typos, so watch out! However I really hope that I included all the details of this lab task.

Oh, and the make files of libs, generated obj folder has 2 subfolders for object files compiled with fPIC and without it.

### Results

You have the compiled library(both dynamic and static) and executable files that can test these libraries.

### Additional tasks

None for now..

