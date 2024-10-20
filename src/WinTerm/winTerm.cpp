#include "WinTerm/winTerm.hpp"
#include "WinTerm/ansi/stdinReader.hpp"
#include "WinTerm/events/queue.hpp"
#include "WinTerm/misc/size.hpp"
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

		stdinReaderThread = std::thread(&winTerm::stdinReader);
	}

	void destroy()
	{
		if(!shouldQuit.load()) {
			std::cout << "programm is attempting to quit before postQuitEvent() was called\r" << std::endl;
			std::cout << "there is a bug somewhere...\r" << std::endl;
		}

		// join event handling thread
		shouldQuit.store(true);
		if(stdinReaderThread .joinable()) stdinReaderThread.join();
		
		// restore original terminal settings
		tcsetattr(STDIN_FILENO , TCSAFLUSH , &origTermios);

		std::cout << "Press Enter to Exit" << std::endl;
		std::cin.get();

		// restore original terminal buffer
		char restoreBufferEsc[9] = "\x1b[?1049l";
		write(STDOUT_FILENO , restoreBufferEsc , sizeof(restoreBufferEsc));

	}


	void postQuitEvent(int returnCode) noexcept
	{
		pushEvent(event(event::QUIT , returnCode));
		shouldQuit.store(true);
	}

	int getEvent(event& e) noexcept {
		popEvent(e);
		if(e.t_ == event::type::QUIT) return 0;
		else return 1;
	}

}
