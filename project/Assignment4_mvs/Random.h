#ifndef RANDOM_H
#define RANDOM_H

#include "Vec3D.h"

/**
 * Provides useful randomization functions.
 */
class Random {
public:
	/**
	 * Returns a random float in the range [0, 1].
	 * @return A random float in the range [0, 1].
	 */
	static float sampleUnit();

	/**
	 * Returns two floating point numbers in the unit disk.
	 * The range is [-1, 1] x [-1, 1], u^2 + v^2 <= 1.
	 * @param[out] u The u component.
	 * @param[out] v The v component.
	 */
	static void sampleUnitDisk(float &u, float &v);

	/**
	 * Returns two floating point numbers in the unit square.
	 * The range is [-1, 1] x [-1, 1].
	 * @param[out] u The u component.
	 * @param[out] v The v component.
	 */
	static void sampleUnitSquare(float &u, float &v);

	/**
	 * Returns a random point on the unit sphere.
	 * The range is [-1, 1] x [-1, 1] x [-1, 1], x^2 + y^2 + z^2 = 1.
	 * @return A point on the unit sphere.
	 */
	static Vec3Df sampleUnitSphere();

	/**
	 * Returns a random point on the unit sphere.
	 * The range is [-1, 1] x [-1, 1] x [-1, 1], x^2 + y^2 + z^2 = 1.
	 * @param[in] normal The normal defining the hemisphere to be sampled.
	 * @return A point on the given hemisphere.
	 */
	static Vec3Df sampleHemisphere(const Vec3Df &normal);
};

#endif