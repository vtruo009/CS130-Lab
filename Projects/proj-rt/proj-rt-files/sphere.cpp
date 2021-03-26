#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
    Hit hit = {0,0,0};
    double descriminant = pow( dot(ray.direction, ray.endpoint - center), 2 ) - dot(ray.direction, ray.direction)*(dot(ray.endpoint - center, ray.endpoint - center) - pow(radius, 2));
    if (descriminant <= 0 ) {
		return {0, 0, 0};
	}
	else {
		double denominator = dot(ray.direction, ray.direction);
		double b_part = dot(ray.direction * -1, ray.endpoint - center);
		double s_part = sqrt(descriminant);
		double t1 = ( b_part + s_part ) / denominator;
		double t2 = ( b_part - s_part ) / denominator;
		if (t1 < 0 && t2 < 0) {
			hit.object = NULL;
		}
		else if (t1 > 0 && t1 < t2) {
			if (t1 <= small_t) {
				hit.object = NULL;
				hit.dist = 0;
				return hit;
			}
			hit.object = this;
			hit.dist = t1;
		}
		else if (t2 > 0 && t2 < t1) {
			if (t2 <= small_t) {
				hit.object = NULL;
				hit.dist = 0;
				return hit;
			}
			hit.object = this;
			hit.dist = t2;
		}
	}
    return hit;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal = point - center;
    //TODO; // compute the normal direction
    normal = normal.normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
