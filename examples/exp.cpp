#include "winTerm.hpp"
#include <string>
#include <unistd.h>

namespace wt = winTerm;

std::string buffer;
unsigned int x = 0;
unsigned int y = 0;

void termProc(wt::msg *msg) {
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
				else if(kbd == wt::keyboard::ARROW_RIGHT) {
					x++;
					wt::postPaintMessage();
				}
				else if(kbd == wt::keyboard::ARROW_DOWN) {
					y++;
					wt::postPaintMessage();
				}
				else if(kbd == wt::keyboard::CTRL_Q) {
					wt::postQuitMessage(0);
				}
			}
			break;
		case wt::message::PAINT:
			{
				constexpr unsigned int height = 35, width = 100;

				wt::canvas *cv = wt::beginPaint(width , height);
				cv->setPosition(x, y);
				cv->setBackground(wt::colour::black);
				cv->setBorder(wt::borderStyle::two);

				cv->addText(" Window Tittle Here " , 0 , width / 2 - 21 / 2 , wt::colour::white,
					wt::colour::black, wt::emphasis::bold);

				cv->addText(buffer, 3, 3, wt::colour::white, wt::colour::black, wt::emphasis::norm);

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


