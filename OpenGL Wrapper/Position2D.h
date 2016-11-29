#pragma once
#include <utility>

#include "Units.h"

namespace Physics
{
	template <int GridSize>
	class Position2D {
		Distance<Meters> x;
		Distance<Meters> y;

		void rot(int n, int *x, int *y, int rx, int ry) {
			if (ry == 0) {
				if (rx == 1) {
					*x = n - 1 - *x;
					*y = n - 1 - *y;
				}

				//Swap x and y
				std::swap(x, y);
			}
		}

	public:
		Position2D(Distance<Meters> x_, Distance<Meters> y_)
			: x(x_), y(y_) {}

		// Convert from a Hilbert Curve Position to 2D Coordinates
		Position2D(int d) {
			int rx, ry, s, t = d;
			int xt, yt = 0;
			for (s = 1; s < GridSize; s *= 2) {
				rx = 1 & (t / 2);
				ry = 1 & (t ^ rx);
				rot(s, &xt, &yt, rx, ry);
				xt += s * rx;
				yt += s * ry;
				t /= 4;
			}

			x = Distance<Meters>(xt);
			y = Distance<Meters>(yt);
		}

		// Get the position's Hilbert Curve Position
		int to_1D() {
			int rx, ry, s, d = 0;
			int xt = (int)x;
			int yt = (int)y;
			for (s = GridSize / 2; s>0; s /= 2) {
				rx = (xt & s) > 0;
				ry = (yt & s) > 0;
				d += s * s * ((3 * rx) ^ ry);
				rot(s, &xt, &yt, rx, ry);
			}
			return d;
		}
	};
}
