#include <cassert>

#include "BRDF.h"
#include "IMaterial.h"

BRDF::BRDF(const IMaterial *material)
: material(material) {
	assert(material);
}