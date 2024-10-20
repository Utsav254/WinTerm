#pragma once
#include <thread>

namespace winTerm
{
	constexpr ssize_t ansiEscSize_ = 64;

	// terminal api thread
	extern std::thread stdinReaderThread;
	extern std::atomic <bool> shouldQuit;

	void stdinReader() noexcept;
}
