#include "WinTerm/events/resize.hpp"
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>

namespace winTerm
{
	void updateTerminalSize()
	{
		struct winsize ws;
		if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws) == 0)
		{
			columns = ws.ws_col;
			rows = ws.ws_row;
		}
		else throw std::runtime_error("unable to fetch window size");
	}

	void signalHandler(int)
	{
		updateTerminalSize();
	}

}
