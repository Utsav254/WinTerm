#include "canvas.hpp"
#include "winTerm.hpp"
#include <string>
#include <unistd.h>
#include <cmath>
#include <vector>

namespace wt = winTerm;

struct vertex{
	float x, y, z;
	wt::colour col;
	vertex(const float x, const float y, const float z, const wt::colour c): x(x), y(y), z(z), col(c){}
};

struct vec3 {
	float x, y, z;
	vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

vec3 rotateX(const vec3& v, float angle) {
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	return vec3(v.x, v.y * cos_a - v.z * sin_a, v.y * sin_a + v.z * cos_a);
}

vec3 rotateY(const vec3& v, float angle) {
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	return vec3(v.x * cos_a + v.z * sin_a, v.y, -v.x * sin_a + v.z * cos_a);
}

vec3 rotateZ(const vec3& v, float angle) {
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	return vec3(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a, v.z);
}

void vs(wt::vsOut& POSITION, wt::colour& col, const vertex& v, const float x_offset, const float y_offset)
{
	(void)x_offset;
	(void)y_offset;
	
	float fov = 1.0f;
	float z_offset = 2.0f;
	float z_proj = v.z + z_offset;
	
	if (z_proj <= 0.1f) z_proj = 0.1f;
	
	POSITION.x = (v.x * fov) / z_proj;
	POSITION.y = (v.y * fov) / z_proj;
	POSITION.z = v.z;
	POSITION.w = 1.0f;
	col = v.col;
}

void fs(wt::colour& COLOUR, const wt::colour& col, const float x_offset, const float y_offset)
{
	(void)x_offset;
	(void)y_offset;
	COLOUR = col;
}

std::vector<vertex> createCube(float rotation_x, float rotation_y, float rotation_z) {
	// Define cube vertices (centered at origin)
	const float size = 0.7f;
	std::vector<vec3> cube_verts = {
		// Front face
		{-size, -size,  size}, // 0
		{ size, -size,  size}, // 1
		{ size,  size,  size}, // 2
		{-size,  size,  size}, // 3
		// Back face
		{-size, -size, -size}, // 4
		{ size, -size, -size}, // 5
		{ size,  size, -size}, // 6
		{-size,  size, -size}  // 7
	};
	
	std::vector<wt::colour> colors = {
		wt::colour::red,
		wt::colour::green,
		wt::colour::blue,
		wt::colour::yellow,
		wt::colour::magenta_fuchsia,
		wt::colour::cyan,
		wt::colour::white,
		wt::colour::red
	};
	
	std::vector<vertex> rotated_verts;
	for (size_t i = 0; i < cube_verts.size(); ++i) {
		vec3 v = cube_verts[i];
		v = rotateX(v, rotation_x);
		v = rotateY(v, rotation_y);
		v = rotateZ(v, rotation_z);
		rotated_verts.emplace_back(v.x, v.y, v.z, colors[i]);
	}
	
	return rotated_verts;
}

std::vector<unsigned int> getCubeWireframeIndices() {
	return {
		// Front face edges
		0, 1, 1, 2, 2, 3, 3, 0,
		// Back face edges  
		4, 5, 5, 6, 6, 7, 7, 4,
		// Connecting front to back
		0, 4, 1, 5, 2, 6, 3, 7
	};
}

int termProc(wt::msg *msg) {
	static float spin_angle = 0.0f;
	
	switch (msg->m) {
		case wt::message::KEYBOARD:
			{
				wt::keyboard kbd = msg->param.kbd;
				if(kbd == wt::keyboard::CTRL_Q) {
					wt::postQuitMessage(0);
					return 0;
				}
				else if(kbd == wt::keyboard::ARROW_RIGHT) {
					spin_angle += 0.05f;
				}
				wt::postPaintMessage();
			}
			break;
		case wt::message::PAINT:
			{
				constexpr unsigned int x = 80, y = 40;
				wt::canvas *cv = wt::beginPaint(x , y);
				cv->setBackground(wt::colour::black);
				cv->setForeground(wt::colour::black);
				cv->setBorder(wt::borderStyle::two, wt::colour::white);
				
				const std::string title = std::format(" Spinning Cube: Angle {:.1f} ", spin_angle);
				cv->addText(title , 0 , (x / 2) - (title.size() / 2), wt::colour::white,
					wt::colour::black, wt::emphasis::bold);
				
				// Create and render the cube with current spin angle
				std::vector<vertex> cube_vbo = createCube(spin_angle, spin_angle, spin_angle);
				std::vector<unsigned int> cube_ibo = getCubeWireframeIndices();
				
				cv->draw(cube_vbo, cube_ibo, wt::drawPrimitives::LINES, &vs, &fs, 0.0f, 0.0f);
				
				wt::endPaint(cv);
			}
			break;
		case wt::message::RESIZE:
		case wt::message::NONE:
		case wt::message::QUIT:
		case wt::message::CREATE:
		case wt::message::DESTROY:
		default:
			break;
    }
	return 0;
}

int main()
{
	wt::initialise();
	wt::msg msg;
	int getEventResult;
	while ((getEventResult = wt::getMessage(&msg)) > 0)
	{
		termProc(&msg);
	}
	wt::destroy();
	if(getEventResult == 0) return static_cast<int>(msg.param.l);
	else return EXIT_FAILURE;
}
