#include "WinTerm/winTerm.hpp"
#include <string>
#include <unistd.h>

using wt = winTerm;

std::string buffer;
unsigned int x = 0;
unsigned int y = 0;

void termProc(handle<wt::msg> msg) {
	switch (msg->m) {
		case wt::message::KEYBOARD:
			{
				wt::keyboard kbd = std::get<wt::keyboard>(msg->param);
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

				handle<wt::canvas> cv = wt::beginPaint(width , height);
				cv->setPosition(x, y);
				cv->setBackground(wt::colour::black);
				cv->setBorder(wt::borderStyle::two);

				cv->addText(" Window Tittle Here " , 0 , width / 2 - 21 / 2 , wt::colour::white,
					wt::colour::black, wt::emphasis::bold);

				cv->addText(buffer, 3, 3, wt::colour::white, wt::colour::black, wt::emphasis::norm);

				wt::endPaint(std::move(cv));
			}
			break;
		case wt::message::RESIZE:
		case wt::message::NONE:
		case wt::message::QUIT:
			break;
    }
}

int main()
{
	wt::initialise();

	handle<wt::msg> msg;
	int getEventResult;

	while ((getEventResult = wt::getMessage(msg)))
	{
		if(msg) termProc(std::move(msg)); // this will be replaces by a dispatcher which will check for nullptr
	}

	wt::destroy();

	if(getEventResult == 0) return static_cast<int>(std::get<long>(msg->param));
	else return EXIT_FAILURE;
}


