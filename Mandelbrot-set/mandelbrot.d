import ldc.attributes;

//This function returns the iterations it takes for the given pixel
//The function it iterates is f(z) = z^2 + C
auto iterateMandelbrot(in real x, in real y) pure nothrow @nogc @safe @fastmath
{
	import std.complex : complex, abs;
	import std.math : log;
	enum max_iters = 500_000U;
	//Skip calculating for some values inside the big bulbs
	if (x >= -1.2 && x <= -0.8 && y <= 0.1)
		return max_iters;
	else if (x >= -0.6 && x <= 0.2 && y <= 0.4)
		return max_iters;
	auto z = 0.complex;
	auto c = complex(x, y);
	uint i;
	for (; i < max_iters && z.re ^^ 2 + z.im ^^ 2 < 4; ++i)
		z = z ^^ 2 + c;
	return i;
}

ubyte[] colourMandelbrot(in uint width, in uint height) @fastmath
{
	import std.parallelism : parallel;
	import std.range : iota;

	ubyte[] output;
	output.length = width * height * 3;

	const PixelWidth = (1.0 + 2.5) / width;
	const PixelHeight = (1.0 + 1.0) / height;

	foreach (pY; iota(height / 2, height).parallel) {
		real y = -1.0 + pY * PixelHeight;
		foreach (pX; 0 .. width) {
			real x = -2.5 + pX * PixelWidth;
			uint i;
			i = iterateMandelbrot(x, y);
			//This doubles the output
			//Makes use of the fact that the set is symmetric about X axis
			foreach (j, ref c; output[3 * pX + 3 * pY * width .. pY * width * 3 + pX * 3 + 3])
				c = (i * 4 + (2 - j) * 8) % ubyte.max;
			foreach (j, ref c; output[3 * pX + 3 * (height - pY) * width .. (height - pY) * width * 3 + pX * 3 + 3])
				c = (i * 4 + (2 - j) * 8) % ubyte.max;
		}
	}
	return output;
}

void main(string[] args)
{
	import std.stdio : File;
	import std.getopt;

	uint width = 720, height = 576;

	auto helpInformation = getopt(
			args,
			"width|w", "width of image", &width,
			"height|h", "height of image", &height);

	if (helpInformation.helpWanted) {
		defaultGetoptPrinter("madelbrot\nUsage : ./madelbrot",
				helpInformation.options);
		return;
	}

	auto file  = File("mandelbrot_" ~ width.to!string ~ "x" ~ height.to!string ~ ".ppm", "wb");
	//ppm Format:
	//P6, comment, rows cols, colors
	file.writef!"P6\n#\n%d %d\n255\n"(width, height);
	file.rawWrite(colourMandelbrot(width, height));
}
