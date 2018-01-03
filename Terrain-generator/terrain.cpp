/*
Copyright 2017-18 Sireesh Kodali

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <random>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstring>

double averageOfNeighbours(std::vector<std::vector<double>> &heightmap, unsigned x, unsigned y)
{
	return (heightmap[x - 1][y] + heightmap[x][y - 1] + heightmap[x + 1][y] + heightmap[x][y + 1] +
			heightmap[x - 1][y - 1] + heightmap[x - 1][y - 1] + heightmap[x+1][y + 1] + heightmap[x + 1][y + 1]) / 8.0;
}

void smoothen(std::vector<std::vector<double>> &heightmap)
{
	for (unsigned i = 1U; i < heightmap.size() - 1U; ++i) {
		for (unsigned j = 1U; j < heightmap.size() - 1U; ++j) {
			double v = averageOfNeighbours(heightmap, i, j);
			heightmap[i][j] = v;
		}
	}
}

void diamondStep(std::vector<std::vector<double>> heightmap, unsigned long side_length, unsigned long half_side, int range)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<double> uniform(-range / 2 - 1, range / 2);

	for (unsigned long i = 0; i < heightmap.size(); i += half_side) {
		for (unsigned long j = (i + half_side) % side_length; j < heightmap.size() - 1; j +=side_length) {

			double avg = heightmap[(i - half_side + heightmap.size() - 1) % heightmap.size()][j] +
				heightmap[(i + half_side) % (heightmap.size() - 1)][j] +
				heightmap[i][(j + half_side) % (heightmap.size() - 1)] +
				heightmap[i][(j - half_side + heightmap.size() - 1) % heightmap.size()];

			avg = avg / 4.0 + uniform(rng);
			heightmap[i][j] = avg;

			if (i == 0)
				heightmap[heightmap.size() - 1][j] = avg;
			if (j == 0)
				heightmap[i][heightmap.size() - 1] = avg;
		}
	}
}

void squareStep(std::vector<std::vector<double>> &heightmap, unsigned long side_length, unsigned long half_side, int range)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<double> uniform(-range / 2 - 1, range / 2);

	for (unsigned long i = 0; i < heightmap.size() - 1 - side_length; i += side_length) {
		for (unsigned long j = 0; j < heightmap.size() - 1 - side_length; j += side_length) {
			double avg = heightmap[i][j] +
				heightmap[i + side_length][j] +
				heightmap[i][j + side_length] +
				heightmap[i + side_length][j + side_length];
			avg /= 4.0;
			heightmap[i + half_side][j + half_side] = avg + uniform(rng);
		}
	}
}
std::vector<std::vector<double>> makeTerrain(unsigned size)
{
	std::vector<std::vector<double>> heightmap(size);
	for (unsigned i = 0; i < size; ++i) {
		std::vector<double> row(size);
		heightmap[i] = row;
	}

	heightmap[0][0] = 100;
	heightmap[0][size - 1] = 100;
	heightmap[size - 1][0] = 100;
	heightmap[size - 1][size - 1] = 100;
	smoothen(heightmap);

	int range = 256;

	for (unsigned long side_length = size - 1; side_length > 1; side_length /= 2, range /= 2) {
		unsigned long half_side = side_length / 2;

		squareStep(heightmap, side_length, half_side, range);
		diamondStep(heightmap, side_length, half_side, range);
	}

	smoothen(heightmap);
	return heightmap;
}

double ** makeTerrainPointer(std::vector<std::vector<double>> &terrain)
{
	auto ptr_terrain = new double*[terrain.size()];
	for (unsigned i = 0; i < terrain.size(); ++i) {
		ptr_terrain[i] = terrain[i].data();
	}
	return ptr_terrain;
}

extern "C" void drawTerrain(size_t length, double ** heightmap);

void draw(std::vector<std::vector<double>> &terrain)
{
	drawTerrain(terrain.size(), makeTerrainPointer(terrain));
}

int main(int argc, char const *argv[])
{
	if (argc > 1) {
		if (strlen(argv[1] == 2) && argv[1][0] == '-' && argv[1][1] == 'l')
			std::cout << R"(lic
Copyright 2017-18 Sireesh Kodali

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
lic)""
	}

	auto terrain = makeTerrain(256);
	draw(terrain);
}
