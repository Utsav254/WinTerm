#include "WinTerm/winTerm.hpp"
#include "WinTerm/privates.hpp"
#include <csignal>
#include <iostream>
#include <unistd.h>

namespace winTerm
{

	void initialise ()
	{
		// get current terminal settings
		tcgetattr(STDIN_FILENO , &winTerm::origTermios_);

		// enable new terminal buffer
		char alternateBufferEsc[9] = "\x1b[?1049h";
		write(STDOUT_FILENO , alternateBufferEsc , sizeof(alternateBufferEsc));
		
		//set new terminal settings
		struct termios rawTermios;
		tcgetattr(STDIN_FILENO , &rawTermios);
		rawTermios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		rawTermios.c_oflag &= ~(OPOST);
		rawTermios.c_cflag |= (CS8);
		rawTermios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		rawTermios.c_cc[VMIN] = 0;
		rawTermios.c_cc[VTIME] = 1;
		tcsetattr(STDIN_FILENO , TCSAFLUSH , &rawTermios);

		updateTerminalSize();

		std::signal(SIGWINCH , &winTerm::signalHandler);

		// move cursor to home position
		char moveCursorHome[4] = "\x1b[H";
		write(STDOUT_FILENO , moveCursorHome , sizeof(moveCursorHome));

		stdinReaderThread_ = std::thread(&winTerm::stdinReader);
	}

	void destroy()
	{
		if(!shouldQuit_) {
			std::cout << "programm is attempting to quit before postQuitEvent() was called\r" << std::endl;
			std::cout << "there is a bug somewhere...\r" << std::endl;
		}

		// join event handling thread
		shouldQuit_ = true;
		if(stdinReaderThread_ .joinable()) stdinReaderThread_.join();
		
		// restore original terminal settings
		tcsetattr(STDIN_FILENO , TCSAFLUSH , &origTermios_);

		std::cout << "Press Enter to Exit" << std::endl;
		std::cin.get();

		// restore original terminal buffer
		char restoreBufferEsc[9] = "\x1b[?1049l";
		write(STDOUT_FILENO , restoreBufferEsc , sizeof(restoreBufferEsc));

	}


	void postQuitEvent(int returnCode) noexcept {
		{
			std::lock_guard<std::mutex> lock(stdinReaderMutex_);
			try
			{
				eventQueue_.push(event(event::type::QUIT , static_cast<long>(returnCode)));
			} catch(const std::overflow_error& e) {
				std::cout << "program not responding, event queue is full" << std::endl;
			}
		}
		shouldQuit_ = true;
	}

	int getEvent(event& e) noexcept {
		int returnVal = 1;
		{
			std::lock_guard<std::mutex> lock(stdinReaderMutex_);
			try {

				eventQueue_.pop(e);
				if(e.t_ == event::type::QUIT) returnVal = 0;

			} catch (const std::underflow_error&) {
				e = event();
				returnVal = -1;
			}
		}
		return returnVal;
	}

}
