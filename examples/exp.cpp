#include "canvas.hpp"
#include "winTerm.hpp"
#include <string>
#include <unistd.h>

namespace wt = winTerm;

int termProc(wt::msg *msg) {
	switch (msg->m) {
		case wt::message::KEYBOARD:
			{
				wt::keyboard kbd = msg->param.kbd;
				if(kbd == wt::keyboard::CTRL_Q) {
					wt::postQuitMessage(0);
				}
				else {
					wt::postPaintMessage();
				}
			}
			break;
		case wt::message::PAINT:
			{
				constexpr unsigned int x = 100, y = 35;
				wt::canvas *cv = wt::beginPaint(x , y);
				cv->setBorder(wt::borderStyle::two);

				static constexpr std::string title = " Window Title ";
				cv->addText(title , 0 , (x / 2) - (title.size() / 2), wt::colour::white,
					wt::colour::black, wt::emphasis::bold);

				std::vector<wt::vertex> vbo;
				vbo.emplace_back( 0.000f,  0.800f, 0.0f, 1.0f, wt::colour::red);
				vbo.emplace_back( 0.760f,  0.248f, 0.0f, 1.0f, wt::colour::green);
				vbo.emplace_back( 0.470f, -0.800f, 0.0f, 1.0f, wt::colour::blue);
				vbo.emplace_back(-0.470f, -0.800f, 0.0f, 1.0f, wt::colour::yellow);
				vbo.emplace_back(-0.760f,  0.248f, 0.0f, 1.0f, wt::colour::magenta_fuchsia);

				const std::vector<unsigned int> ibo = {
					0, 4, 1,
					0, 1, 2,
					0, 2, 3,  
					0, 3, 4
				};

				cv->drawPrimitives(vbo, ibo, wt::drawPrimitives::TRIANGLES);

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


