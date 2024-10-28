#include "WinTerm/events/event.hpp"
#include "WinTerm/events/messages.hpp"
#include "generics/boundedQueue.hpp"
#include "WinTerm/ansi/stdinReader.hpp"

namespace winTerm
{
	// eventQueue not exposed 
	// holds all events
	boundedQueue<message , messageQueueSize> messageQueue;

	void postQuitMessage(int returnCode)
	{
		shouldQuit.store(true);
		while(!messageQueue.emplace(message::QUIT , returnCode)) {
			messageQueue.pop(shouldQuit);
		}
	}

	bool postPaintMessage()
	{
		return messageQueue.emplace(message::PAINT , 0);
	}

	int getMessage(std::unique_ptr<message>& msg) noexcept {

		msg = messageQueue.pop(shouldQuit);

		if(msg == nullptr) return -1;
		else if(msg->t == message::QUIT) return 0;
		else return 1;
	}

	void endMessage()
	{
		messageQueue.clear();
	}
}
