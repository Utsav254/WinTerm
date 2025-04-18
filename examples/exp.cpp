#include "WinTerm/winTerm.hpp"
#include <string>

std::string buffer;

void termProc(handle<winTerm::msg> msg) {
	switch (msg->m) {
		case winTerm::message::KEYBOARD:
			{
				winTerm::keyboard kbd = std::get<winTerm::keyboard>(msg->param);
				if(kbd >= 32 && kbd < 127) {
					if(buffer.size() % 94 == 0 && buffer.size() != 0){
						buffer += '\n';
					}
					buffer += char(kbd);
					winTerm::postPaintMessage();
				}
				else if(kbd == winTerm::keyboard::CTRL_Q) {
					winTerm::postQuitMessage(0);
				}
			}
			break;
		case winTerm::message::PAINT:
			{
				const unsigned int height = 35, width = 100;

				handle<winTerm::canvas> cv = winTerm::beginPaint(width , height);
				cv->setBackground(winTerm::colour::black);
				cv->setBorder(winTerm::borderStyle::two);

				cv->addText(" Window Tittle Here " , 0 , width / 2 - 21 / 2 , winTerm::colour::white,
					winTerm::colour::black, winTerm::emphasis::bold);

				cv->addText(buffer, 3, 3, winTerm::colour::white, winTerm::colour::black, winTerm::emphasis::norm);

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


