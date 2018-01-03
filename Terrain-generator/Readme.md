# Terrain Generator

This program uses the [diamond-square](https://en.wikipedia.org/wiki/Diamond-square_algorithm) algorithm
to genrate a 3d mountaious terrain. The terrain is then rendered to the screen with `OpenGL` and `glut`.

The purpose of the program is to show that a seemingly complex terrain can be quickly generated with
a rather simple algorithm. It was inspired by a scene in [The Pixar Story](https://www.themoviedb.org/movie/15302-the-pixar-story) which showed a mountain being generated from a triangle.

There are two parts to this program. The first one, which generates the terrain was written in `D` and
later translated to `C++` due to difficulty of linking `C` libraries with `D` programs on Windows.

The second part is the renderer. It is written in `C` with `OpenGL` and `GLUT`.

### Compilation

  * `OpenGL` and `freeglut` libraries are required, for rendering the terrain

  1. Compiling `C` rendering part:

     ```sh
     cc terrain.c -std=c11 -O3 -o terrain_gl.o -I/path/to/OpenGL -I/path/to/GLUT -c
     ```
 
     _On macOS_:

     ```sh
     cc terrain.c -std=c11 -O3 -o terrain_gl.o -framework OpenGL -framework GLUT -c
     ```

  2. Compiling terrain generating part

     * Note: You will need to compile ony one of the following (C++ or D files):

     1. with `D`:

        ```sh
        dmd terrain.d terrain_gl.o -O -release -inline -of=terrain -L/path/to/OpenGl -L/path/to/freeglut
        ```
       
        _On macOS_:

           ````sh
           dmd terrain.d terrain_gl.o -O -release -inline -of=terrain -L-framework -LOpenGl -L-framework -LGLUT
           ````

     2. with `C++`

        ```sh
        c++ -std=c++11 -O3 terrain.cpp -L/path/to/OpenGL -L/path/to/freeglut terrain.cpp -o terrain
        ```

        _On macOS_:

           ```sh
           c++ terrain.cpp -O3 -std=c++11 terrain_gl.o -o terrain -framework OpenGL -framework GLUT
           ```

### Usage

```sh
./terrain
```

Once the terrain is on screen,

|Key           |Action                    |
|:------------:|:------------------------:|
|up arrow key  |zoom in                   |
|down arrow key| zoom out                 |
|r or R        | keep rotating the terrain|
|esc key       |quit                      |
