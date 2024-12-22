#include "WinTerm/ansi/stdinReader.hpp"
#include "WinTerm/events/event.hpp"
#include "WinTerm/events/messages.hpp"
#include <pthread.h>

namespace winTerm
{
	// reader thread and atomic booleans for thread control
	std::thread stdinReaderThread;
	std::atomic <bool> shouldQuit(false);

	void readParseEscapeSequence();

	void stdinReader() noexcept
	{
		pthread_setname_np(pthread_self() , "stdinreader");
		while(!shouldQuit.load())
		{
			char c = 0;
			read(STDIN_FILENO , &c , 1);
			if(c == 27) {
				readParseEscapeSequence();
			}
			else if(c != 0) {
				messageQueue.emplace(message::KEYBOARD , static_cast<keyboard>(c));
			}
		}
	}


	// TODO: find a better way to do this 
	// a better faster more maintable way 
	// flex bison parser??
	// throws for unhandled escape sequences
	void readParseEscapeSequence()
	{
		char seqBuffer[ansiEscSize_] = {0};
		ssize_t n = read(STDIN_FILENO, seqBuffer, sizeof(seqBuffer));
		// handle error hase
		if(n < 0) throw std::runtime_error("was not able to read from stdin , return code: " + std::to_string(n));
		if(n >= ansiEscSize_) throw std::runtime_error("ansi escape sequence overflow");
		
		if(n == 0) {
			messageQueue.emplace(message::KEYBOARD , keyboard::ESCAPE);
			return;
		}
		else {
			switch(seqBuffer[0])
			{
				// CONTROL SEQUENCE INTRODUCED (CSI) handles either [ or O else throw
				case '[':
					// numerical and tilda refer to keyboard events:
					if(seqBuffer[1] >= '0' && seqBuffer[1] <= '9' && seqBuffer[2] == '~') {
						switch(seqBuffer[1]) {
							case '1': messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
								break;
							case '3': messageQueue.emplace(message::KEYBOARD , keyboard::DELETE);
								break;
							case '4': messageQueue.emplace(message::KEYBOARD , keyboard::END);
								break;
							case '5': messageQueue.emplace(message::KEYBOARD , keyboard::PAGE_UP);
								break;
							case '6': messageQueue.emplace(message::KEYBOARD , keyboard::PAGE_DOWN);
								break;
							case '7': messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
								break;
							case '8': messageQueue.emplace(message::KEYBOARD , keyboard::END);
								break;
							default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
						}
					}
					//alphabeta refer to keyboard events:
					else {
						switch(seqBuffer[1])
						{
							case 'A': messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_UP);
								break;
							case 'B': messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_DOWN);
								break;
							case 'C': messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_RIGHT);
								break;
							case 'D': messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_LEFT);
								break;
							case 'H': messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
								break;
							case 'F': messageQueue.emplace(message::KEYBOARD , keyboard::END);
								break;
							default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
						}
					}
					break;

				case 'O':
					switch(seqBuffer[1])
					{
						case 'H': messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
							break;
						case 'F': messageQueue.emplace(message::KEYBOARD , keyboard::END);
							break;
						case 'P': messageQueue.emplace(message::KEYBOARD , keyboard::FN_1);
							break;
						case 'Q': messageQueue.emplace(message::KEYBOARD , keyboard::FN_2);
							break;
						case 'R': messageQueue.emplace(message::KEYBOARD , keyboard::FN_3);
							break;
						case 'S': messageQueue.emplace(message::KEYBOARD , keyboard::FN_4);
							break;
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
					break;
				default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
			}
		}
	}
}
