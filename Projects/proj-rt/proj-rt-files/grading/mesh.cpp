#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    //TODO;
    double min_t = std::numeric_limits<double>::max();
    double distance = 0;
    Hit hit = {0,0,0};
    
    if (part < 0) {
		for(unsigned int i = 0; i < triangles.size(); ++i) {
            if(Intersect_Triangle(ray, i, distance)) {
                if(distance < min_t) {
                    min_t = distance;
                    hit.object = this;
                    hit.dist = distance;
                    hit.part = i;
                }
            }
        } 
	}
	else {
		if(Intersect_Triangle(ray, part, distance)) {
            hit.object = this;
			hit.dist = distance;
			hit.part = part;
		}
	}
    return hit;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    //TODO;
    ivec3 curr_triangle = triangles[part];
    
    // get sides then cross
    vec3 AB = vertices[curr_triangle[1]] - vertices[curr_triangle[0]];
    vec3 AC = vertices[curr_triangle[2]] - vertices[curr_triangle[0]];
    
    vec3 cross_prod = cross(AB, AC).normalized();
    
    return cross_prod;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    //TODO;
    ivec3 points = triangles[tri];
    vec3 x = vertices[points[1]] - vertices[points[0]];
    vec3 y = vertices[points[2]] - vertices[points[0]];
    vec3 z = ray.endpoint -vertices[triangles[tri][0]];
    
    if (!dot(ray.direction, this->Normal(ray.direction, tri))) {
		return false;
	}
	
	double beta = dot(cross(ray.direction, y), z) / dot(cross(ray.direction, y), x);
	double gamma = dot(cross(ray.direction, x), z) / dot(cross(ray.direction, x), y);
	double alpha = 1 - beta - gamma;
	
	dist = -dot(cross(x, y), z) / dot(cross(x, y), ray.direction);
	
	if (alpha > weight_tolerance && beta > weight_tolerance && gamma > weight_tolerance) {
		return true;
	}
    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
