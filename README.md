# FileEmbedder

FileEmbedder is a C++ program for embedding files into stand-alone executables.

## How it works ?

FileEmbedder is converting all files from given folder into .cpp files and provides simple class for accesing them from c++ code.

## Included example

In VS2015 solution there is a simple example program which shows how to create self unpacking archive/exe with FileEmbedder

**Building example program**
- Open Visual Studio solution
- Build FileEmbedder project, build scripts should copy example data to build folder
- Run FileEmbedder, it will serialize example data to .cpp files
- Build Test project, scripts will copy generated files to project directory
- Now there should be test.exe in build folder which works as self unpacking archive
