#include "winTerm.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>

namespace wt = winTerm;

std::string buffer;
unsigned int x = 70;
unsigned int y = 35;

int termProc(wt::msg *msg) {
	switch (msg->m) {
		case wt::message::KEYBOARD:
			{
				wt::keyboard kbd = msg->param.kbd;
				if(kbd >= 32 && kbd < 127) {
					if(buffer.size() % 94 == 0 && buffer.size() != 0){
						buffer += '\n';
					}
					buffer += char(kbd);
					wt::postPaintMessage();
				}
				else if(kbd == wt::keyboard::CTRL_Q) {
					wt::postQuitMessage(0);
				}
			}
			break;
		case wt::message::PAINT:
			{
				wt::canvas *cv = wt::beginPaint(x , y);
				// cv->setPosition(x, y);
				cv->setBackground(wt::colour::black);
				cv->setBorder(wt::borderStyle::two);

				std::string title = std::format(" Window Title Here {},{}", x, y);

				cv->addText(title , 0 , x / 2 - 21 / 2 , wt::colour::white,
					wt::colour::black, wt::emphasis::bold);

				cv->addText(buffer, 3, 3, wt::colour::white, wt::colour::black, wt::emphasis::norm);

				wt::endPaint(cv);
			}
			break;
		case wt::message::RESIZE:
			std::cout << "hello world resize" << std::endl;
			x = msg->param.l >> 16;
			y = msg->param.l & 0xffff;
			winTerm::postPaintMessage();
			break;
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


