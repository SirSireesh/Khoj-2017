# [Mandelbrot Set](https://en.wikipedia.org/wiki/Mandelbrot_set)

This program was made to generate the Mandelbrot set at arbitrary (reasonable) resolutions, quickly.
It was written in `D` since it is easy to use to quickly write program which is reasonably performant.

The program outputs a file with name `madelbrot_widthxheight.ppm`, where `width` and `height` are the size of the image.
This file is an image of the Madelbrot set. 

### Compilation

`ldc` >= 1.4 is needed.

command line:

`ldc mandelbrot.d -O3 -release -inline`

### Usage

`./mandelbrot -width=xxx -height=yyy` for width and height

followed by opening the `.ppm` file created.
