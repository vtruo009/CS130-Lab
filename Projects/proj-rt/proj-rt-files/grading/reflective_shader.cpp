#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    //TODO; // determine the color
    
    vec3 view_ray = ray.direction;
    vec3 reflectionDir = 2 * dot(-view_ray, normal) * normal + view_ray;
    
    Ray r = Ray(intersection_point, reflectionDir);
    
    if (recursion_depth < world.recursion_depth_limit) {
		color = (1 - reflectivity) * color + reflectivity * world.Cast_Ray(r, ++recursion_depth);
	}
	else {
		color *= (1 - reflectivity);
	}

    return color;
}
