#include <cassert>

#include "Reflection.h"
#include "IMaterial.h"

Reflection::Reflection(const IMaterial *material)
: material(material) {
	assert(material);
}