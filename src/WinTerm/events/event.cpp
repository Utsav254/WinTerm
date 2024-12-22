#include "WinTerm/events/event.hpp"
#include "WinTerm/events/messages.hpp"
#include "WinTerm/ansi/stdinReader.hpp"
#include "WinTerm/render/render.hpp"

namespace winTerm
{
	// eventQueue not exposed 
	// holds all events
	channel<message , messageQueueSize> messageQueue;

	void postQuitMessage(int returnCode)
	{
		shouldQuit.store(true);
		renderQueue.emplace(canvas::canvasMessage::END);
		messageQueue.emplace(message::QUIT , returnCode);
	}

	void postPaintMessage()
	{
		messageQueue.emplace(message::PAINT , 0);
	}

	int getMessage(handle<message>& msg) noexcept {

		msg = messageQueue.pop();

		if(msg == nullptr) return -1;
		else if(msg->t == message::QUIT) return 0;
		else return 1;
	}
}
