#pragma once
#include "event.hpp"

namespace winTerm
{
	void initialise();
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
