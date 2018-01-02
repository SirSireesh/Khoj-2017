# [Sierpinski triangles](https://en.wikipedia.org/wiki/Sierpinski_triangle)

This program simply draws the triangles upto a smallest length. Then it zooms
in until the triangles are too big. It then repeats the process.

The point is to show Sierpinski triangles and their infinite nature as a quick introduction to fractals

### Compile Instructions

A `C99` compliant compiler is needed. Also `freeglut` and `OpenGL` libraries are needed.

With `clang` you can do:

`cc -std=c99 -O3 sierpinski.c -L/path/to/freeglut -L/path/to/OpenGL -o sierpinski`
