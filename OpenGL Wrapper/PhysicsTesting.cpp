#include <iostream>
#include <algorithm>
#include <vector>

#include "PhysicsBody.h"
#include "Units.h"


#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <random>
#include <utility>
#include <chrono>

//rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
	if (ry == 0) {
		if (rx == 1) {
			*x = n - 1 - *x;
			*y = n - 1 - *y;
		}

		//Swap x and y
		int t = *x;
		*x = *y;
		*y = t;
	}
}

//convert (x,y) to d
int xy2d(int n, int x, int y) {
	int rx, ry, s, d = 0;
	for (s = n / 2; s>0; s /= 2) {
		rx = (x & s) > 0;
		ry = (y & s) > 0;
		d += s * s * ((3 * rx) ^ ry);
		rot(s, &x, &y, rx, ry);
	}
	return d;
}

//convert d to (x,y)
void d2xy(int n, int d, int *x, int *y) {
	int rx, ry, s, t = d;
	*x = *y = 0;
	for (s = 1; s<n; s *= 2) {
		rx = 1 & (t / 2);
		ry = 1 & (t ^ rx);
		rot(s, x, y, rx, ry);
		*x += s * rx;
		*y += s * ry;
		t /= 4;
	}
}


std::vector<std::pair<int, int>> gen_random_pairs(const int n)
{
	std::vector<int> xs(n);
	std::iota(xs.begin(), xs.end(), 0);

	std::vector<int> ys(n);
	std::iota(ys.begin(), ys.end(), 0);

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(xs.begin(), xs.end(), g);
	std::shuffle(ys.begin(), ys.end(), g);

	std::vector<std::pair<int, int>> pairs;

	for (int i = 0; i < n; i++) {
		pairs.emplace_back(std::pair<int, int>(xs[i], ys[i]));
	}

	return pairs;
}

void print_hilbert_grid(const int N)
{
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			std::cout << xy2d(N, x, y) << "\t";
		}
		std::cout << "\n";
	}

	std::cout << "\n";
}

void time_hilbert_mapping() {
	const int N = 1024;
	const int coords = 10000;
	auto pairs = gen_random_pairs(coords);
	std::chrono::high_resolution_clock clock;
	std::vector<int> ds;

	ds.reserve(coords);

	auto start = clock.now();

	for (auto& pair : pairs) {
		ds.push_back(xy2d(N, pair.first, pair.second));
	}

	std::sort(ds.begin(), ds.end());

	auto end = clock.now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Mapping completed in: " << duration.count() << '\n';

	std::for_each(pairs.begin(), pairs.begin() + 10, [=](std::pair<int, int> pair) {
		std::cout << "(" << pair.first << "," << pair.second << ")" << " -> ";
		std::cout << xy2d(N, pair.first, pair.second) << '\n';
	});

	std::cout << "End" << std::endl;
}

void physics_tests()
{
	for (int i = 0; i < 6; i++) {
		print_hilbert_grid(pow(2,i));
	}
}