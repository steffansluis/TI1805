#include <cstdlib>
#include <math.h>

#include "Constants.h"
#include "Random.h"

float Random::sampleUnit() {
	// Return a random float in the range [0, 1]
	return rand() / static_cast<float>(RAND_MAX);
}

void Random::sampleUnitDisk(float &u, float &v) {
	do {
		// Get two random floats in the range [0, 1]
		u = rand() / static_cast<float>(RAND_MAX);
		v = rand() / static_cast<float>(RAND_MAX);

		// Map the numbers to [-1, 1]
		u = (u - 0.5f) * 2.0f;
		v = (v - 0.5f) * 2.0f;
	} while (u * u + v * v > 1.0f);
}

void Random::sampleUnitSquare(float &u, float &v) {
	// Get two random floats in the range [0, 1]
	u = rand() / static_cast<float>(RAND_MAX);
	v = rand() / static_cast<float>(RAND_MAX);
}

Vec3Df Random::sampleUnitSphere() {
	float phi;
	float cosTheta;
	float sinTheta;

	// Calculate random phi in range [0, 2pi]
	phi = rand() / static_cast<float>(RAND_MAX) * Constants::TwoPi;

	// Calculate random sin(theta) in range [-1, 1]
	sinTheta = ((rand() / static_cast<float>(RAND_MAX)) - 0.5f) * 2.0f;

	// Calculate cos(theta)
	cosTheta = sqrtf(1.0f - sinTheta * sinTheta);

	// Convert the polar coordinates to cartesian
	return Vec3Df(cosTheta * cosf(phi), cosTheta * sinf(phi), sinTheta);
}

Vec3Df Random::sampleHemisphere(const Vec3Df &normal) {
	// Get a random point on the unit sphere.
	Vec3Df point = Random::sampleUnitSphere();

	// If the point is on the opposite side of the hemisphere,
	// mirror the point.
	if (Vec3Df::dotProduct(normal, point) < 0.0f)
		point = -point;

	return point;
}