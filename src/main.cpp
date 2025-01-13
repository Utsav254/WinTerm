#include "WinTerm/winTerm.hpp"

void termProc(handle<winTerm::msg> msg) {
	switch (msg->m) {
		case winTerm::message::KEYBOARD:
			switch(std::get<winTerm::keyboard>(msg->param))
			{
				case winTerm::keyboard::CTRL_Q:
					winTerm::postQuitMessage(0);
					break;
				default:
					winTerm::postPaintMessage();
					break;
			}
			break;
		case winTerm::message::PAINT:
			{
				const unsigned int height = 35, width = 100;

				auto cv = winTerm::beginPaint(width , height);
				cv->setBackground(winTerm::colour::blue);
				cv->setBorder(winTerm::borderStyle::two);

				cv->addText(" Window Tittle Here " , 0 , width / 2 - 21 / 2 , winTerm::colour::white,
					winTerm::colour::red, winTerm::emphasis::bold);

				cv->drawRect(winTerm::rect(2 , 2 , 97 , 20), winTerm::colour::green, static_cast<winTerm::borderStyle>(0) , false);

				winTerm::endPaint(std::move(cv));
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

	handle<winTerm::msg> msg;
	int getEventResult;

	while ((getEventResult = winTerm::getMessage(msg)))
	{
		if(msg) termProc(std::move(msg)); // this will be replaces by a dispatcher which will check for nullptr
	}

	winTerm::destroy();
	
	if(getEventResult == 0) return static_cast<int>(std::get<long>(msg->param));
	else return EXIT_FAILURE;
}


