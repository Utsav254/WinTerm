#include <iostream>
#include <unistd.h>
#include "WinTerm/winTerm.hpp"

void termProc(const winTerm::event& e) {
	switch (e.t_) {
		case winTerm::event::KEYBOARD:
			switch(e.param_) {
				case static_cast<long>(winTerm::keyboard::CTRL_Q):
					winTerm::postQuitEvent(0);
					break;
				case '\r':
					{
						char carriageReturn[] = "\n\r";
						write(STDOUT_FILENO , &carriageReturn , 2);
					}
					break;
				case 127:
					{
						char backspace[] = "\b \b";
						write(STDOUT_FILENO , &backspace , 3);
					}
					break;
				default:
					{
						char c = static_cast<char>(e.param_);
						write(STDOUT_FILENO , &c , 1);
					}
					break;
			}
			break;
		case winTerm::event::RESIZE:
			std::cout << "resize: " << winTerm::rows << " , " << winTerm::columns  << "\r" << std::endl;
			break;
		case winTerm::event::NONE:
        case winTerm::event::QUIT:
            break;
    }
}

int main()
{
	winTerm::initialise();

	winTerm::event e;
	int getEventResult;
	while ((getEventResult = winTerm::getEvent(e)))
	{
		termProc(e);
	}

	winTerm::destroy();
	
	if(getEventResult == 0) return e.param_;
	else return EXIT_FAILURE;
}


