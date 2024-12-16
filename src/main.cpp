#include <unistd.h>
#include "WinTerm/render/render.hpp"
#include "WinTerm/winTerm.hpp"
#include "WinTerm/render/canvas.hpp"
#include "WinTerm/render/misc.hpp"
#include <cwchar>

void termProc(std::unique_ptr<winTerm::message> msg) {
	switch (msg->t) {
		case winTerm::message::KEYBOARD:
			switch(std::get<winTerm::keyboard>(msg->param)) {
				case winTerm::keyboard::CTRL_Q:
					winTerm::postQuitMessage(0);
					break;
				case winTerm::keyboard::ARROW_RIGHT:
					break;
				case winTerm::keyboard::ARROW_DOWN:
					break;
				default:
					break;
			}
			winTerm::postPaintMessage();
			break;
        case winTerm::message::PAINT:
			{
				unsigned int height = 35, width = 100;
				std::unique_ptr<winTerm::canvas> cv = winTerm::beginPaint(width , height);
				cv->setBackground(static_cast<fmt::color>(0x1a2c4a));
				cv->setBorder(winTerm::borderStyle::DOUBLE);
				cv->addText(" Window Tittle Here " , 0 , width / 2 - 21 / 2 , fmt::color::white,
					static_cast<fmt::color>(0x1a2c4a) , fmt::emphasis::bold);
				cv->drawRect(winTerm::rect(2 , 2 , 97 , 20) , fmt::color::slate_gray , winTerm::borderStyle::THIN , false);
				endPaint(std::move(cv));
			}
			break;
		case winTerm::message::RESIZE:
		case winTerm::message::NONE:
        case winTerm::message::QUIT:
          	break;
        }
}

int main()
{
	winTerm::initialise();

	std::unique_ptr<winTerm::message> msg;
	int getEventResult;

	while ((getEventResult = winTerm::getMessage(msg)))
	{
		if(msg) termProc(std::move(msg)); // this will be replaces by a dispatcher which will check for nullptr
	}

	winTerm::destroy();
	
	if(getEventResult == 0) return static_cast<int>(std::get<long>(msg->param));
	else return EXIT_FAILURE;
}


