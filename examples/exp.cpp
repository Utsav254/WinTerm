#include "canvas.hpp"
#include "winTerm.hpp"
#include <string>
#include <unistd.h>

namespace wt = winTerm;

struct vertex{
	float x, y, z;
	wt::colour col;

	vertex(const float x, const float y, const float z, const wt::colour c): x(x), y(y), z(z), col(c){}
};

void vs(wt::vsOut& POSITION, wt::colour& col, const vertex& v, const float x_offset, const float y_offset)
{
	POSITION.x = v.x + x_offset;
	POSITION.y = v.y + y_offset;
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

int termProc(wt::msg *msg) {
	static float x_offset = 0.0f, y_offset = 0.0f;
	switch (msg->m) {
		case wt::message::KEYBOARD:
			{
				wt::keyboard kbd = msg->param.kbd;
				if(kbd == wt::keyboard::CTRL_Q) {
					wt::postQuitMessage(0);
					return 0;
				}
				else if (kbd == wt::keyboard::ARROW_LEFT) {
					x_offset += 0.1f;
				}
				else if(kbd == wt::keyboard::ARROW_DOWN) {
					y_offset += 0.1;
				}
				wt::postPaintMessage();
			}
			break;
		case wt::message::PAINT:
			{
				constexpr unsigned int x = 100, y = 35;
				wt::canvas *cv = wt::beginPaint(x , y);

				cv->setBackground(wt::colour::white);
				cv->setBorder(wt::borderStyle::two);

				const std::string title = std::format(" Window Title: {}, {} ", x_offset, y_offset);
				cv->addText(title , 0 , (x / 2) - (title.size() / 2), wt::colour::black,
					wt::colour::white, wt::emphasis::bold);

				std::vector<vertex> vbo;
				vbo.emplace_back( 0.000f,  0.800f, 0.0f, wt::colour::red);
				vbo.emplace_back( 0.760f,  0.248f, 0.0f, wt::colour::green);
				vbo.emplace_back( 0.470f, -0.800f, 0.0f, wt::colour::blue);
				vbo.emplace_back(-0.470f, -0.800f, 0.0f, wt::colour::yellow);
				vbo.emplace_back(-0.760f,  0.248f, 0.0f, wt::colour::magenta_fuchsia);

				const std::vector<unsigned int> ibo = {
					0, 4, 1,
					0, 1, 2,
					0, 2, 3,  
					0, 3, 4
				};

				cv->draw(vbo, ibo, wt::drawPrimitives::TRIANGLE_STRIP, &vs, &fs, x_offset, y_offset);

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


