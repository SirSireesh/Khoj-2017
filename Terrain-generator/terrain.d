double[][] makeTerrain(uint size) @safe
{
	import std.math : isPowerOf2, nextPow2;
	if (!isPowerOf2(size - 1))
		size = nextPow2(size);

	double[][] heightmap;
	heightmap.length = size;
	foreach (ref row; heightmap)
		row.length = size;

	foreach (ref row; heightmap)
		row[] = 0.0;

	heightmap[0][0] = 100;
	heightmap[0][$ - 1] = 100;
	heightmap[$ - 1][0] = 100;
	heightmap[$ - 1][$ - 1] = 100;
	heightmap = heightmap.smoothen;
	int range = 64;

	for (ulong side_length = heightmap.length - 1; side_length > 1; side_length /= 2, range /= 2) {
		ulong half_side = side_length / 2;

		heightmap.squareStep(side_length, half_side, range);
		heightmap.diamondStep(side_length, half_side, range);
	}

	foreach (row; heightmap[1 .. $ - 1])
		row[1 .. $ - 1] *= 1.5;

	return heightmap.smoothen;
}

private auto smoothen(double[][] heightmap) @safe
{
	for (uint i = 1U; i < heightmap.length - 1U; i++) {
		for (uint j = 1U; j < (heightmap.length - 1U); j++){
			double v = getNeighboursAVG(heightmap, i, j);
			heightmap[i][j] = v;
		}
	}
	return heightmap;
}

private double getNeighboursAVG(ref double[][] heightmap, uint x, uint y) @safe
{
	return (heightmap[x - 1][y] + heightmap[x][y - 1] + heightmap[x + 1][y] + heightmap[x][y + 1] +
			heightmap[x - 1][y - 1] + heightmap[x - 1][y - 1] + heightmap[x+1][y + 1] + heightmap[x + 1][y + 1]) / 8;
}

void diamondStep(ref double[][] heightmap, ulong side_length, ulong half_side, int range) @safe
{
	import std.random : Random, unpredictableSeed, uniform;
	import std.math : isInfinity, isNaN;
	auto gen = Random(unpredictableSeed);

	for (ulong i = 0; i < heightmap.length; i += half_side) {
		for (ulong j = (i + half_side) % side_length; j < heightmap.length - 1; j +=side_length) {

			double avg = heightmap[(i - half_side + $ - 1) % ($ - 1)][j] +
				heightmap[(i + half_side) % ($ - 1)][j] +
				heightmap[i][(j + half_side) % ($ - 1)] +
				heightmap[i][(j - half_side + $ - 1) % ($ - 1)];

			avg = avg / 4.0 + uniform!"[]"(-range / 2, range, gen);
			if (isInfinity(avg) || isNaN(avg))
				assert(false, "Something went wrong, avg is NaN or inf");
				//avg = 1.0 + uniform!"[]"(-range, range, gen);
			heightmap[i][j] = avg;

			if (i == 0)
				heightmap[$ - 1][j] = avg;
			if (j == 0)
				heightmap[i][$ - 1] = avg;
		}
	}
}

void squareStep(ref double[][] heightmap, ulong side_length, ulong half_side, int range) @safe
{
	import std.random;
	auto gen = Random(unpredictableSeed);

	for (ulong i = 0; i < heightmap.length - 1 - side_length; i += side_length) {
		for (ulong j = 0; j < heightmap.length - 1 - side_length; j += side_length) {
			double avg = heightmap[i][j] +
				heightmap[i + side_length][j] +
				heightmap[i][j + side_length] +
				heightmap[i + side_length][j + side_length];
			avg /= 4.0;
			heightmap[i + half_side][j + half_side] = avg + uniform!"[]"(-range / 2, range);
		}
	}
}

double ** makeTerrainPointer(ref double[][] terrain) @trusted pure nothrow
{
	import core.stdc.string : memcpy;
	auto ptr_terrain = new double*[terrain.length];
	foreach (i; 0 .. terrain.length) {
		ptr_terrain[i] = terrain[i].ptr;
	}
	return ptr_terrain.ptr;
}

extern(C) void drawTerrain(size_t length, double ** heightmap) @nogc nothrow @safe;

void draw(double[][] terrain) nothrow @safe
{
	import std.math : isNaN, isInfinity;

	drawTerrain(terrain.length, terrain.makeTerrainPointer);
}

void main()
{
	makeTerrain(2 ^^ 7).draw;
}