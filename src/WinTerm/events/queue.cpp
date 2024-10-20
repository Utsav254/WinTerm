#include "WinTerm/events/queue.hpp"
#include "WinTerm/events/event.hpp"
#include "generics/boundedQueue.hpp"
#include <mutex>
#include <iostream>
#include <condition_variable>


namespace winTerm
{
	
	// eventQueue not exposed 
	// holds all events
	boundedQueue<event , eventQueueSize> eventQueue;

	// mutex must be held before modifying the message queue
	std::mutex mutex;
	// conditional variable to notify pop function that message is available to pop
	std::condition_variable condVar;

	void pushEvent(const event& evnt) noexcept
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			try
			{
				eventQueue.push(evnt);
				condVar.notify_one();		
			}
			catch(const std::overflow_error& e) {
				std::cout << "program not responding, event queue is full" << std::endl;
			}
	
		}
	}

	void popEvent(event& evnt) noexcept
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			condVar.wait(lock , []{ return !eventQueue.isEmpty(); });
			try {
				eventQueue.pop(evnt);
			} catch (const std::underflow_error&) {
				evnt = event();
			}
		}
	}

}
