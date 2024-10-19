#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <termios.h>
#include <sys/ioctl.h>
#include "generics/boundedQueue.hpp"
#include "WinTerm/event.hpp"

namespace winTerm
{
	void stdinReader() noexcept;
	void updateTerminalSize();
	void signalHandler(int);

	constexpr ssize_t ansiEscSize_ = 64;
	constexpr std::size_t eventQueueSize = 64;
	
	//terminal window size variables
	extern int rows , columns;

	// terminal settings
	extern struct termios origTermios_;
	
	// terminal api thread
	extern std::thread stdinReaderThread_;
	extern std::mutex stdinReaderMutex_;
	extern std::atomic <bool> shouldQuit_;
	
	// event queue
	extern boundedQueue<event , eventQueueSize> eventQueue_;
}
