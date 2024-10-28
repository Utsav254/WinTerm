#include "WinTerm/winTerm.hpp"
#include "WinTerm/ansi/stdinReader.hpp"
#include "WinTerm/misc/size.hpp"
#include "WinTerm/render/render.hpp"
#include <csignal>
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace winTerm
{
	struct termios origTermios;
	int columns , rows;

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
		if(!shouldQuit.load()) {
			std::cout << "programm is attempting to quit before postQuitEvent() was called\r" << std::endl;
			std::cout << "there is a bug somewhere...\r" << std::endl;
		}

		// set quitting flag high
		shouldQuit.store(true);

		// notify all awaiting condvars
		endMessage();
		endRender();

		if(stdinReaderThread .joinable()) stdinReaderThread.join();
		if(renderThread .joinable()) renderThread.join();
		
		// restore original terminal settings
		tcsetattr(STDIN_FILENO , TCSAFLUSH , &origTermios);

		std::cout << "Press Enter to Exit" << std::endl;
		std::cin.get();

		// restore original terminal buffer
		char restoreBufferEsc[9] = "\x1b[?1049l";
		write(STDOUT_FILENO , restoreBufferEsc , sizeof(restoreBufferEsc));

	}
}
