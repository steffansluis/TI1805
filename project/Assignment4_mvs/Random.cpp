#define _CRT_RAND_S

#include <cstdlib>
#include <cmath>
#include <ctime>

#include "Constants.h"
#include "Random.h"

#ifndef WIN32
__thread bool Random::initialized;
__thread unsigned int Random::seed;
#endif

unsigned int Random::rand() {
#if !defined(_OPENMP) || !defined(__GLIBC__)
	return ::rand();
#else
 #ifdef WIN32
	// rand_s is threadsafe and generates an unsigned integer in the range [0, UINT_MAX]
	unsigned int rnd;
	rand_s(&rnd);

	// Return a random number
	return rnd % RAND_MAX;
 #else
	if (!Random::initialized) {
		Random::initialized = true;
		Random::seed = time(NULL);
	}

	// Return a random number
	return rand_r(&seed);
 #endif
#endif
}

float Random::randUnit() {
#if !defined(_OPENMP) || !defined(__GLIBC__)
	// Return a random float in the range [0, 1]
	return ::rand() / static_cast<float>(RAND_MAX);
#else
#ifdef WIN32
	// rand_s is threadsafe and generates an unsigned integer in the range [0, UINT_MAX]
	unsigned int rnd;
	rand_s(&rnd);

	// Return a random float in the range [0, 1]
	return rnd / static_cast<float>(UINT_MAX);
 #else
	if (!Random::initialized) {
		Random::initialized = true;
		Random::seed = time(NULL);
	}

	// Return a random float in the range [0, 1]
	return rand_r(&seed) / static_cast<float>(RAND_MAX);
 #endif
#endif
}

void Random::sampleUnitDisk(float &u, float &v) {
	// Generate a random angle in the range [0, 2pi] 
	float s = Constants::TwoPi * Random::randUnit();

	// Generate a random radius in the range [0, 2]
	float t = Random::randUnit() + Random::randUnit();

	// Map the radius to [-1, 1]
	float r = t > 1.0f ? 2.0f - t : t;

	// Use the random radius and angle to generate a random point
	u = r * cosf(s);
	v = r * sinf(s);
}

void Random::sampleUnitSquare(float &u, float &v) {
	// Get two random floats in the range [0, 1]
	u = Random::randUnit();
	v = Random::randUnit();
}

Vec3Df Random::sampleUnitSphere() {
	float phi;
	float cosTheta;
	float sinTheta;

	// Calculate random phi in range [0, 2pi]
	phi = Random::randUnit() * Constants::TwoPi;

	// Calculate random sin(theta) in range [-1, 1]
	sinTheta = (Random::randUnit() - 0.5f) * 2.0f;

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
