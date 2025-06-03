#include "canvas.hpp"
#include "defs.hpp"
#include "winTerm.hpp"
#include <string>
#include <vector>
#include <chrono>

namespace wt = winTerm;

struct vertex
{
	wt::vec4 pos;
	wt::vec4 col;
	vertex(const float x, const float y, const float z, const float w,
			const float r, const float g, const float b, const float a):	
		pos(x, y, z, w), col(r, g, b, a) {}
};

void vs(wt::vsOut& POSITION, wt::vec4& col, const vertex& v, const wt::mat4 rotate)
{
	rotate.mul(v.pos, POSITION);
	col = v.col;
}

void fs(wt::vec4& COLOUR, const wt::vec4& col, const wt::mat4 rotate)
{
	(void)rotate;
	COLOUR = col;
}

int termProc(wt::msg *msg) {
	static float spin_angle = 0.0f;
	static wt::mat4 rotation_mat;

	static std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

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

				std::vector<vertex> vbo;
				vbo.emplace_back(0.0f, 0.7f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
				vbo.emplace_back(-0.7f, -0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
				vbo.emplace_back(0.7f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

				const std::vector<unsigned int> ibo = {2,1,0};
				wt::make_rotation_z(spin_angle, rotation_mat);
				
				start = std::chrono::high_resolution_clock::now();
				cv->draw(vbo, ibo, wt::drawPrimitives::TRIANGLES, &vs, &fs, rotation_mat);
				end = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration<float, std::milli>(end - start).count();
				const std::string title = std::format(" Frame Time: {:.2f} ms ", duration);
				cv->addText(title , 0 , (x / 2) - (title.size() / 2), wt::colour::white,
					wt::colour::black, wt::emphasis::bold);
				
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

