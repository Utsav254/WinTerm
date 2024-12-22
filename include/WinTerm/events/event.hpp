#pragma once
#include "generics/channel.hpp"
#include "messages.hpp"
#include <cstddef>

namespace winTerm
{
	constexpr std::size_t messageQueueSize = 64;

	extern channel<message , messageQueueSize> messageQueue;
	
	// use to bind message to quitting program
	// set the return Code if required
	// if unable to post, will force push by popping extra event and pushing
	void postQuitMessage(int returnCode);

	// use to bind message to a painting operation
	// return true on success and false on failure
	void postPaintMessage();
	
	// use to get events from event queue
	// return value is 0 if QUIT message
	// positive if no error
	// negative if error
	int getMessage(handle<message>& msg) noexcept;

	// used to clear out the message queue while quitting
	// notifies all condVars to stop awaiting
	void endMessage();
}
