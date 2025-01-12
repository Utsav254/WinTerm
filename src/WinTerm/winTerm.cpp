#include "WinTerm/winTerm.hpp"
#include "WinTerm/ansi/stdinReader.hpp"
#include "WinTerm/render/render.hpp"
#include "WinTerm/events/resize.hpp"
#include <csignal>
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace winTerm
{
	struct termios origTermios;
	int columns , rows;

	char *oldLocale;

	void initialise ()
	{
		// get current terminal settings
		tcgetattr(STDIN_FILENO , &origTermios);

		// enable new terminal buffer
		char alternateBufferEsc[9] = "\x1b[?1049h";
		write(STDOUT_FILENO , alternateBufferEsc , sizeof(alternateBufferEsc));
		
		//set new terminal settings
		struct termios rawTermios;
		tcgetattr(STDIN_FILENO , &rawTermios);
		rawTermios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		rawTermios.c_oflag &= ~(OPOST);
		rawTermios.c_cflag |= (CS8);
		rawTermios.c_lflag &= ~(ECHO | ICANON | IEXTEN);
		rawTermios.c_cc[VMIN] = 0;
		rawTermios.c_cc[VTIME] = 1;
		tcsetattr(STDIN_FILENO , TCSAFLUSH , &rawTermios);

		oldLocale = std::setlocale(LC_ALL, "");

		updateTerminalSize();

		// std::signal(SIGWINCH , &signalHandler);

		// move cursor to home position
		char moveCursorHome[4] = "\x1b[H";
		write(STDOUT_FILENO , moveCursorHome , sizeof(moveCursorHome));
		
		renderThread = std::thread(&winTerm::renderCanvas);
		stdinReaderThread = std::thread(&winTerm::stdinReader);
	}

	void destroy()
	{
		if(stdinReaderThread .joinable()) stdinReaderThread.join();
		if(renderThread .joinable()) renderThread.join();

		std::setlocale(LC_ALL, oldLocale);
		
		// restore original terminal settings
		tcsetattr(STDIN_FILENO , TCSAFLUSH , &origTermios);

		std::cout << "Press Enter to Exit" << std::endl;
		std::cin.get();

		// restore original terminal buffer
		char restoreBufferEsc[9] = "\x1b[?1049l";
		write(STDOUT_FILENO , restoreBufferEsc , sizeof(restoreBufferEsc));

	}
}
