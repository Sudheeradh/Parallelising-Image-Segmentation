The following program is implementation of color based image segmentation usign K-means clustering algorithm.
The code utilises C and OpenMP.
This particluar code is wriiten on run on MacOS, since MacOS doesn't have gcc and uses clang, which doesn't support OpenMP.
Hence llvg package must be installed using homebrew or other package installer.
After installing the package, the following steps must be followed to run the code:

The project is compiled using the make command.
Launching ```make all``` will produce:

-> *serial.out*: the serial version of the program.

-> *omp.out*: the parallel version implemented using OpenMP.

After compiling using the make command, the code is run as showin the following examples:

-> ```./serial.out -k 4 imgs/test.jpeg```: to execute the serial program with
  four clusters on a test image.

-> ```./omp.out -k 4 -t 4 imgs/test.jpeg```: to execute the parallel version
  with four clusters using four CPU threads.



Note: 
To run the program on non MacOS devices, replace the make file in this folder with the one from alt folder and follow the above steps(from ```make all```).

External libraries have been used for handling the I/O of images from the following repo:
https://github.com/nothings/stb