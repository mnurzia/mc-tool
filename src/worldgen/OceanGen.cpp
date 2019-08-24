#include "OceanGen.hpp"

namespace WorldGen {

	void OceanGen::init(int64_t seed) {
		for (int32_t i = 0; i < 512; i++) {
			d[i] = 0;
		}

		a = 0;
		b = 0;
		c = 0;

		JavaRandom::setSeed(&seed);

		a = JavaRandom::nextDouble(&seed) * 256.0;
		b = JavaRandom::nextDouble(&seed) * 256.0;
		c = JavaRandom::nextDouble(&seed) * 256.0;

		for (int32_t i = 0; i < 256; i++) {
			d[i] = i;
		}

		for (int32_t i = 0; i < 256; i++) {
			int32_t n3 = JavaRandom::nextInt(&seed, 256 - i) + i;
			int32_t n4 = d[i];
			d[i] = d[n3];
			d[n3] = n4;
			d[i + 256] = d[i];
		}
	}

	static double lerp(const double part, const double from, const double to) {
		return from + part * (to - from);
	}

	/* Table of vectors to cube edge centres (12 + 4 extra), used for ocean PRNG */
	const double cube_edge_x[] = { 1.0,-1.0, 1.0,-1.0, 1.0,-1.0, 1.0,-1.0, 0.0, 0.0, 0.0, 0.0,  1.0, 0.0,-1.0, 0.0 };
	const double cube_edge_y[] = { 1.0, 1.0,-1.0,-1.0, 0.0, 0.0, 0.0, 0.0, 1.0,-1.0, 1.0,-1.0,  1.0,-1.0, 1.0,-1.0 };
	const double cube_edge_z[] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0,-1.0,-1.0, 1.0, 1.0,-1.0,-1.0,  0.0, 1.0, 0.0,-1.0 };

	static double indexedLerp(int idx, const double d1, const double d2, const double d3) {
		idx &= 0xf;
		return cube_edge_x[idx] * d1 + cube_edge_y[idx] * d2 + cube_edge_z[idx] * d3;
	}

	double OceanGen::getTemp(double d1, double d2, double d3) {
		d1 += a;
		d2 += b;
		d3 += c;
		int32_t i1 = (int32_t)d1 - (int32_t)(d1 < 0);
		int32_t i2 = (int32_t)d2 - (int32_t)(d2 < 0);
		int32_t i3 = (int32_t)d3 - (int32_t)(d3 < 0);
		d1 -= i1;
		d2 -= i2;
		d3 -= i3;
		double t1 = d1 * d1 * d1 * (d1 * (d1 * 6.0 - 15.0) + 10.0);
		double t2 = d2 * d2 * d2 * (d2 * (d2 * 6.0 - 15.0) + 10.0);
		double t3 = d3 * d3 * d3 * (d3 * (d3 * 6.0 - 15.0) + 10.0);

		i1 &= 0xff;
		i2 &= 0xff;
		i3 &= 0xff;

		int32_t a1 = d[i1] + i2;
		int32_t a2 = d[a1] + i3;
		int32_t a3 = d[a1 + 1] + i3;
		int32_t b1 = d[i1 + 1] + i2;
		int32_t b2 = d[b1] + i3;
		int32_t b3 = d[b1 + 1] + i3;

		double l1 = indexedLerp(d[a2], d1, d2, d3);
		double l2 = indexedLerp(d[b2], d1 - 1, d2, d3);
		double l3 = indexedLerp(d[a3], d1, d2 - 1, d3);
		double l4 = indexedLerp(d[b3], d1 - 1, d2 - 1, d3);
		double l5 = indexedLerp(d[a2 + 1], d1, d2, d3 - 1);
		double l6 = indexedLerp(d[b2 + 1], d1 - 1, d2, d3 - 1);
		double l7 = indexedLerp(d[a3 + 1], d1, d2 - 1, d3 - 1);
		double l8 = indexedLerp(d[b3 + 1], d1 - 1, d2 - 1, d3 - 1);

		l1 = lerp(t1, l1, l2);
		l3 = lerp(t1, l3, l4);
		l5 = lerp(t1, l5, l6);
		l7 = lerp(t1, l7, l8);

		l1 = lerp(t2, l1, l3);
		l5 = lerp(t2, l5, l7);

		return lerp(t3, l1, l5);
	}

}