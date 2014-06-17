#include <math.h>

#include "BaseTriangleGeometry.h"
#include "Constants.h"
#include "mesh.h"
#include "Random.h"
#include "RayIntersection.h"
#include "SurfacePoint.h"

void BaseTriangleGeometry::preprocess() {
	// Get the vertices for the triangle
	Vec3Df vertex0 = this->getVertex0();
	Vec3Df vertex1 = this->getVertex1();
	Vec3Df vertex2 = this->getVertex2();

	// The normal-vector of the triangle is the vector that is orthogonal to the plane on which
	// all three vectors lie. 
	// This can be calculated by taking the cross product of two aribtrary vectors that
	// lie on the plane defined by the triangle.
	this->tangent = vertex1 - vertex0;
	this->tangent.normalize();
	this->bitangent = vertex2 - vertex0;
	this->bitangent.normalize();
	this->normal = Vec3Df::crossProduct(this->tangent, this->bitangent);
	this->normal.normalize();
}

// @Author Bas Boellaard
// This method calculates the intersection between the ray originating from 'origin' and having
// 'dir' as its direction.
//
// Logic herein relies heavily on the slides from college as well as data from the following source:
// https://courses.cs.washington.edu/courses/cse457/09sp/lectures/triangle_intersection.pdf
std::shared_ptr<const RayIntersection> BaseTriangleGeometry::calculateIntersection(const Vec3Df &origin, const Vec3Df &dir) const {
	// Get the vertices for the triangle
	Vec3Df vertex0 = this->getVertex0();
	Vec3Df vertex1 = this->getVertex1();
	Vec3Df vertex2 = this->getVertex2();

	float dot_dir_normal = Vec3Df::dotProduct(dir, this->normal);

	// If this angle between the ray and surface normal is positive, then this is the wrong side of the triangle 
	// (i.e. this side is supposed to be the 'inside' of something. It can either be ignored (culling) or negated.
	// ??: Would dot_dir_normal need to be negated if we do not cull it? 
	if (dot_dir_normal >= 0.0f)
	{
		if (Constants::ENABLE_CULLING)
		{
			return nullptr;
		}
		else if (dot_dir_normal >= -1e-6f)
		{
			return nullptr;
		}
	}

	// to calculate the length of the ray that goes towards the triangle, we can calculate 
	// D = n * v_0
	// we can take either v_0, v_1 or v_2; since n is orthogonal it should not matter.
	float d = Vec3Df::dotProduct(this->normal, vertex0);

	// The ray can be calculated as follows:
	// R(t) = origin + t * direction
	// the origin is `origin` and direction is `dest`. The length of the vector is now known as well. 
	//
	// The 't' component can be calculated as follows:
	// t = ( D - dot(origin, normal) ) / ( dot(direction, origin) )
	float dot_origin_normal = Vec3Df::dotProduct(origin, normal);

	float t = (d - dot_origin_normal) / dot_dir_normal;

	// we now know enough to calculate the entire ray.
	// R(t) = origin + t * direction
	Vec3Df hitPoint = origin + t * dir;
	// although called 'ray', this is actually the vector of the intersection-point between 
	// the plane of the triangle and the ray. 

	// finally, we need to check if this ray is actually inside of the triangle. 
	if (this->calculateRayInsideTriangle(hitPoint, normal, vertex0, vertex1, vertex2))
	{
		auto intersection = std::make_shared<RayIntersection>();

		// since the ray-length is less than the already stored data (or has not been stored yet), we 
		// can put the data for the intersection into the object.
		intersection->hitPoint = hitPoint;
		intersection->geometry = this->shared_from_this();

		// also store the original data here.
		intersection->origin = origin;
		intersection->direction = dir;
		intersection->distance = t;

		return intersection;
	}

	return nullptr;
}

std::shared_ptr<const SurfacePoint> BaseTriangleGeometry::getSurfacePoint(std::shared_ptr<const RayIntersection> intersection) const {
	auto surface = std::make_shared<SurfacePoint>();

	surface->geometry = this->shared_from_this();
	surface->point = intersection->hitPoint;
	surface->normal = this->normal;

	// Calculate the hitpoint in local space
	Vec3Df localPoint = intersection->hitPoint - this->getVertex0();

	// Calculates the barycentric coordinates of hit point.
	surface->texCoords[0] = Vec3Df::dotProduct(localPoint, this->tangent);
	surface->texCoords[1] = Vec3Df::dotProduct(localPoint, this->bitangent);

	return surface;
}

std::shared_ptr<const SurfacePoint> BaseTriangleGeometry::getRandomSurfacePoint() const {
	auto surface = std::make_shared<SurfacePoint>();
	float u, v;

	// Get two numbers in the range [0, 1]
	Random::sampleUnitSquare(u, v);

	// Calculate the square root of the first number
	float sqrtU = sqrtf(u);

	// Interpolate the vertices of the triangle in such a 
	// way that this provides uniform sampling.
	// Source: http://www.cs.princeton.edu/~funk/tog02.pdf section 4.2
	Vec3Df point = 
		(1.0f - sqrtU) * this->getVertex0() +
		(sqrtU * (1.0f - v)) * this->getVertex1() +
		(sqrtU * v) * this->getVertex2();

	surface->geometry = this->shared_from_this();
	surface->point = point;
	surface->normal = this->normal;

	// Calculates the barycentric coordinates of hit point.
	surface->texCoords[0] = Vec3Df::dotProduct(point, this->tangent);
	surface->texCoords[1] = Vec3Df::dotProduct(point, this->bitangent);

	return surface;
}

BoundingBox BaseTriangleGeometry::getBoundingBox() const {
	// Construct an empty bounding box
	BoundingBox result = BoundingBox();

	// Insert all three vertices
	result.includePoint(this->getVertex0());
	result.includePoint(this->getVertex1());
	result.includePoint(this->getVertex2());

	return result;
}

// @Author: Bas Boellaard
// This method takes 4 arguments. 
// intersection: this is the coordinate of the ray R(t) = origin + t * direction 
//    where the ray intersects with the coordinate plane of the triangle
// normal: this is the normal of the material. This is the vector that stands orthogonal 
//    to the plane of the triangle.
// v0, v1, v2: these are the coordinates of the triangle.
bool BaseTriangleGeometry::calculateRayInsideTriangle(const Vec3Df &hitPoint, const Vec3Df &normal, const Vec3Df &v0, const Vec3Df &v1, const Vec3Df &v2) {
	// There are three conditions that need to be true for the intersection to be within the three vertices.
	// (v0 - v2) X (intersect - v2) * normal >= 0
	// (v1 - v0) X (intersect - v0) * normal >= 0
	// (v2 - v1) X (intersect - v1) * normal >= 0
	// in all three cases, a different 'edge' is calculated and checked. Q must be within each edge. Only then 
	// is the intersection actually inside of the triangle. 
	Vec3Df v0_v2 = v0 - v2;
	Vec3Df v1_v0 = v1 - v0;
	Vec3Df v2_v1 = v2 - v1;

	Vec3Df q_v2 = hitPoint - v2;
	Vec3Df q_v0 = hitPoint - v0;
	Vec3Df q_v1 = hitPoint - v1;

	Vec3Df cross_2 = Vec3Df::crossProduct(v0_v2, q_v2);
	Vec3Df cross_0 = Vec3Df::crossProduct(v1_v0, q_v0);
	Vec3Df cross_1 = Vec3Df::crossProduct(v2_v1, q_v1);

	if (Vec3Df::dotProduct(cross_2, normal) < 0)
	{
		return false;
	}

	if (Vec3Df::dotProduct(cross_0, normal) < 0)
	{
		return false;
	}

	if (Vec3Df::dotProduct(cross_1, normal) < 0)
	{
		return false;
	}

	// since it has passed all checks, the intersection lies within the triangle
	return true;
}