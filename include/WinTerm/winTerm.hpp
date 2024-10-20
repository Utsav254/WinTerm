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
	
	// use to bind event to quitting program
	// set the return Code if required
	void postQuitEvent(int returnCode) noexcept;
	
	// use to get events from event queue
	// return value is 0 if QUIT message
	// positive if no error
	// negative if error
	int getEvent(event& e) noexcept;
};
