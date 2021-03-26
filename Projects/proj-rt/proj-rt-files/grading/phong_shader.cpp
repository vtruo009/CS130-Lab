#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
	// I = Ra(La) + Rd(Ld)max(n dot 1, 0) + Rs(Ls)max(cos theta, 0) ^ alpha
    vec3 color = world.ambient_color * world.ambient_intensity * color_ambient; //ambient
    
    vec3 lightDir, reflectionDir, L;
    
	for (unsigned int i = 0; i < world.lights.size(); ++i) {
		lightDir = world.lights.at(i)->position - intersection_point;
		reflectionDir = 2.0 * dot(normal, lightDir) * normal - lightDir;
		L= world.lights.at(i)->Emitted_Light(lightDir);

		if (world.enable_shadows) {
			Ray shadow_ray = Ray(intersection_point, lightDir);
			Hit shadow_hit = world.Closest_Intersection(shadow_ray);
			
			if(!shadow_hit.object || shadow_hit.dist > lightDir.magnitude()) {
				color = color
						+ L*color_diffuse*std::max(dot(normal, lightDir.normalized()), 0.0)
						+ L*color_specular*pow(std::max(dot(-ray.direction, reflectionDir.normalized()), 0.0) , specular_power);
			}
		}
		else {
			color = color
					+ L*color_diffuse*std::max(dot(normal, lightDir.normalized()), 0.0)
					+ L*color_specular*pow(std::max(dot(-ray.direction, reflectionDir.normalized()), 0.0) , specular_power);
		}
	}
    return color;
}
