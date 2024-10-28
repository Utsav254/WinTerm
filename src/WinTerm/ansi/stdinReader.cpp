#include "WinTerm/ansi/stdinReader.hpp"
#include "WinTerm/events/event.hpp"
#include "WinTerm/events/messages.hpp"

namespace winTerm
{
	// reader thread and atomic booleans for thread control
	std::thread stdinReaderThread;
	std::atomic <bool> shouldQuit(false);

	std::unique_ptr<message> readParseEscapeSequence();

	void stdinReader() noexcept
	{
		while(!shouldQuit.load())
		{
			char c = 0;
			read(STDIN_FILENO , &c , 1);
			if(c == 27) {
				messageQueue.push(readParseEscapeSequence());
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
	std::unique_ptr<message> readParseEscapeSequence()
	{
		char seqBuffer[ansiEscSize_] = {0};
		ssize_t n = read(STDIN_FILENO, seqBuffer, sizeof(seqBuffer));
		// handle error hase
		if(n < 0) throw std::runtime_error("was not able to read from stdin , return code: " + std::to_string(n));
		if(n >= ansiEscSize_) throw std::runtime_error("ansi escape sequence overflow");
		
		if(n == 0) {
			return std::make_unique<message>(message::KEYBOARD , keyboard::ESCAPE);
		}
		else {
			switch(seqBuffer[0])
			{
				// CONTROL SEQUENCE INTRODUCED (CSI) handles either [ or O else throw
				case '[':
					// numerical and tilda refer to keyboard events:
					if(seqBuffer[1] >= '0' && seqBuffer[1] <= '9' && seqBuffer[2] == '~') {
						switch(seqBuffer[1]) {
							case '1': return std::make_unique<message>(message::KEYBOARD , keyboard::HOME);
							case '3': return std::make_unique<message>(message::KEYBOARD , keyboard::DELETE);
							case '4': return std::make_unique<message>(message::KEYBOARD , keyboard::END);
							case '5': return std::make_unique<message>(message::KEYBOARD , keyboard::PAGE_UP);
							case '6': return std::make_unique<message>(message::KEYBOARD , keyboard::PAGE_DOWN);
							case '7': return std::make_unique<message>(message::KEYBOARD , keyboard::HOME);
							case '8': return std::make_unique<message>(message::KEYBOARD , keyboard::END);
							default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
						}
					}
					//alphabeta refer to keyboard events:
					else {
						switch(seqBuffer[1])
						{
							case 'A': return std::make_unique<message>(message::KEYBOARD , keyboard::ARROW_UP);
							case 'B': return std::make_unique<message>(message::KEYBOARD , keyboard::ARROW_DOWN);
							case 'C': return std::make_unique<message>(message::KEYBOARD , keyboard::ARROW_RIGHT);
							case 'D': return std::make_unique<message>(message::KEYBOARD , keyboard::ARROW_LEFT);
							case 'H': return std::make_unique<message>(message::KEYBOARD , keyboard::HOME);
							case 'F': return std::make_unique<message>(message::KEYBOARD , keyboard::END);
							default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
						}
					}
					break;

				case 'O':
					switch(seqBuffer[1])
					{
						case 'H': return std::make_unique<message>(message::KEYBOARD , keyboard::HOME);
						case 'F': return std::make_unique<message>(message::KEYBOARD , keyboard::END);
						case 'P': return std::make_unique<message>(message::KEYBOARD , keyboard::FN_1);
						case 'Q': return std::make_unique<message>(message::KEYBOARD , keyboard::FN_2);
						case 'R': return std::make_unique<message>(message::KEYBOARD , keyboard::FN_3);
						case 'S': return std::make_unique<message>(message::KEYBOARD , keyboard::FN_4);
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
					break;
				default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
			}
		}
	}
}
