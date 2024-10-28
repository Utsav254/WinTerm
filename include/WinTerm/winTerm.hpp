#pragma once
#include "WinTerm/events/event.hpp"

namespace winTerm
{
	// size of full terminal window
	extern int columns , rows;

	// initialise library
	// will start an alternate buffer and turn off many terminal settings
	// will also set std::terminate to destroy ... 
	// programmer will be unable to set std::terminate due to this
	// might throw if certain terminal operations are not successful
	void initialise();

	// will join all library threads
	// restore terminal settings and old terminal buffer
	// must be called at exit for proper cleanup
	void destroy();
};
