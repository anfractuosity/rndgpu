rndgpu - GPU based random number generator
==========================================

Compile the OpenCL code, simply by running 'make'.

Compile the program to pack the data into binary, using:
**gcc packforent.c -lm -o packforent**

Run the random number generator which outputs 0's and 1's as text
using:
**./rng > rnd**

Then convert the text output, to a binary file rnd.bin, using:
**./packforent**

Then use 'ent' to gauge the randomness:
**ent rnd.bin**
