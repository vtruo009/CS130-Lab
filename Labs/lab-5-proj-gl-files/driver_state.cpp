#include "driver_state.h"
#include <cstring>
#include <limits>

driver_state::driver_state()
{
}

driver_state::~driver_state()
{
    delete [] image_color;
    delete [] image_depth;
}

// This function should allocate and initialize the arrays that store color and
// depth.  This is not done during the constructor since the width and height
// are not known when this class is constructed.
void initialize_render(driver_state& state, int width, int height)
{
    state.image_width=width;
    state.image_height=height;
    state.image_color = new pixel[width * height];
    state.image_depth = new float[width * height];
    
    for (int i = 0; i < (width * height); ++i) {
		state.image_color[i] = make_pixel(0,0,0);
		state.image_depth[i] = std::numeric_limits<float>::max();
	}
    //std::cout<<"TODO: allocate and initialize state.image_color and state.image_depth."<<std::endl;
}

// This function will be called to render the data that has been stored in this class.
// Valid values of type are:
//   render_type::triangle - Each group of three vertices corresponds to a triangle.
//   render_type::indexed -  Each group of three indices in index_data corresponds
//                           to a triangle.  These numbers are indices into vertex_data.
//   render_type::fan -      The vertices are to be interpreted as a triangle fan.
//   render_type::strip -    The vertices are to be interpreted as a triangle strip.
void render(driver_state& state, render_type type)
{
    //std::cout<<"TODO: implement rendering."<<std::endl;
    data_geometry *object = new data_geometry[3]; //one for each vertex
    data_vertex in{}; //for vertex_shader
    float * temp = state.vertex_data;
    bool is_first = true;
    
    switch(type) {
		case render_type::triangle: {
			for (int i = 0, j = 0; i < state.num_vertices; ++i, ++j) {
				object[j].data = in.data = temp;
				state.vertex_shader(in, object[j], state.uniform_data);
				if (j == 2) {
					rasterize_triangle(state, object[0], object[1], object[2]);
					j = -1;
				}
				temp += state.floats_per_vertex;
			}
			break;
		}
		
		
		case render_type::indexed: { //need to use num_triangles instead of vertices
			for (int i = 0; i < state.num_triangles * 3; i += 3) {
				for (int j = 0; j < 3; ++j) {
					temp = &state.vertex_data[state.index_data[i + j] * state.floats_per_vertex];
					object[j].data = in.data = temp;
					state.vertex_shader(in, object[j], state.uniform_data);
				}
				rasterize_triangle(state, object[0], object[1], object[2]);
			}
			break;
		}
		
		case render_type::fan: {
			for (int i = 0, j = 0; i < state.num_vertices; ++i, ++j) { //j needs to increase at the same time as i just like in triangle
				if (is_first) {
					object[j].data = in.data = state.vertex_data;
					--i;
					is_first = false;
				}
				else {
					temp += state.floats_per_vertex;
					object[j].data = in.data = temp;
				}
				
				state.vertex_shader(in, object[j], state.uniform_data);
				if (j == 2) {
					rasterize_triangle(state, object[0], object[1], object[2]);
					temp -= state.floats_per_vertex;
					j = -1;
					--i;
					is_first = true;
				}
			}
			break;
		}
		
		case render_type::strip: {
			for (int i = 0, j = 0; i < state.num_vertices; ++i, ++j) { //similar to triangle case
				object[j].data = in.data = temp;
				state.vertex_shader(in, object[j], state.uniform_data);
				if (j == 2) {
					rasterize_triangle(state, object[0], object[1], object[2]);
					temp -= state.floats_per_vertex * 2;
					j = -1;
					i -= 2;
				}
				temp += state.floats_per_vertex;
			}
			break;
		}
		
		default:
			break;
	}
	delete [] object;
}


// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry& v0,
    const data_geometry& v1, const data_geometry& v2,int face)
{
    if(face==1)
    {
        rasterize_triangle(state, v0, v1, v2);
        return;
    }
    std::cout<<"TODO: implement clipping. (The current code passes the triangle through without clipping them.)"<<std::endl;
    clip_triangle(state, v0, v1, v2,face+1);
}

// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry& v0,
    const data_geometry& v1, const data_geometry& v2)
{
    //std::cout<<"TODO: implement rasterization"<<std::endl;
    int x[3], y[3], z[3];
    data_output output;
    float *data = new float[MAX_FLOATS_PER_VERTEX];
    data_fragment frag{data};
    
    x[0] = (state.image_width / 2.0) * (v0.gl_Position[0]/v0.gl_Position[3]) + ((state.image_width / 2.0) - 0.5);
    x[1] = (state.image_width / 2.0) * (v1.gl_Position[0]/v1.gl_Position[3]) + ((state.image_width / 2.0) - 0.5);
    x[2] = (state.image_width / 2.0) * (v2.gl_Position[0]/v2.gl_Position[3]) + ((state.image_width / 2.0) - 0.5);
    
    y[0] = (state.image_height / 2.0) * (v0.gl_Position[1]/v0.gl_Position[3]) + ((state.image_height / 2.0) - 0.5);
    y[1] = (state.image_height / 2.0) * (v1.gl_Position[1]/v1.gl_Position[3]) + ((state.image_height / 2.0) - 0.5);
    y[2] = (state.image_height / 2.0) * (v2.gl_Position[1]/v2.gl_Position[3]) + ((state.image_height / 2.0) - 0.5);
    
    z[0] = (state.image_height / 2.0) * (v0.gl_Position[2]/v0.gl_Position[3]) + ((state.image_width / 2.0) - 0.5);
    z[1] = (state.image_height / 2.0) * (v1.gl_Position[2]/v1.gl_Position[3]) + ((state.image_width / 2.0) - 0.5);
    z[2] = (state.image_height / 2.0) * (v2.gl_Position[2]/v2.gl_Position[3]) + ((state.image_width / 2.0) - 0.5);
    
    float areaABC = 0.5 * ((x[1]*y[2] - x[2]*y[1]) + (x[2]*y[0] - x[0]*y[2]) + (x[0]*y[1] - x[1]*y[0]));
    
    int min_x = std::min(std::min(x[0], x[1]), x[2]);
    int min_y = std::min(std::min(y[0], y[1]), y[2]);
    int max_x = std::max(std::max(x[0], x[1]), x[2]);
    int max_y = std::max(std::max(y[0], y[1]), y[2]);
    
    if (min_x < 0) {
        min_x = 0;
	}
    if (min_y < 0) {
        min_y = 0;
	}
    if (max_x >= state.image_width) {
		max_x = state.image_width;
	}
    if (max_y >= state.image_height) {
        max_y = state.image_height;
	}

	for (int i = min_y; i <= max_y; ++i) {
		for (int j = min_x; j <= max_x; ++j) {
			float a = (0.5 * ((x[1] * y[2] - x[2] * y[1]) + (y[1] - y[2])*j + (x[2] - x[1])*i)) / areaABC;
			float b = (0.5 * ((x[2] * y[0] - x[0] * y[2]) + (y[2] - y[0])*j + (x[0] - x[2])*i)) / areaABC;
			float g = (0.5 * ((x[0] * y[1] - x[1] * y[0]) + (y[0] - y[1])*j + (x[1] - x[0])*i)) / areaABC;
			
			float z_val = a * z[0] + b * z[1] + g * z[2];
			
			if ( a >= 0 && b >= 0 && g >= 0) {
				if (z_val < state.image_depth[j + i * state.image_width]) {
					state.image_depth[j + i * state.image_width] = z_val;
					for (int k = 0; k < state.floats_per_vertex; ++k) {
						switch(state.interp_rules[k]) {
							case interp_type::flat: {
								frag.data[k] = v0.data[k];
								break;
							}
							case interp_type::noperspective: {
								frag.data[k] = (a * v0.data[k]) + (b * v1.data[k]) + (g * v2.data[k]);
								break;
							}
							case interp_type::smooth: {
								float gour_const =
									a / v0.gl_Position[3] +
									b / v1.gl_Position[3] +
									g / v2.gl_Position[3];
								
								a /= (gour_const * v0.gl_Position[3]);
								b /= (gour_const * v1.gl_Position[3]);
								g /= (gour_const * v2.gl_Position[3]);
							
								break;
							}
							default:
								break;
						}
					}
				
					state.fragment_shader(frag, output, state.uniform_data);
					
					state.image_color[j + (i * state.image_width)] = make_pixel(
						(output.output_color[0] * 255),
						(output.output_color[1] * 255),
						(output.output_color[2] * 255)
					);
				}
			}
		}
	}
	
	delete [] data;
}

