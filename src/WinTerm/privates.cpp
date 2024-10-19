#include "WinTerm/privates.hpp"
#include <iostream>

namespace winTerm
{
	int rows = 0 , columns = 0;

	// terminal settings
	struct termios origTermios_;
	
	// terminal api thread
	std::thread stdinReaderThread_;
	std::mutex stdinReaderMutex_;
	std::atomic <bool> shouldQuit_;
	
	// event queue
	boundedQueue<event , eventQueueSize> eventQueue_;
}

namespace winTerm
{

event readParseEscapeSequence();

void stdinReader() noexcept
{
	while(!shouldQuit_)
	{
		char c = 0;
		read(STDIN_FILENO , &c , 1);
		{
			std::lock_guard<std::mutex> lock(stdinReaderMutex_);
			try
			{
				if(c == 27) {
					eventQueue_.push(readParseEscapeSequence());
				}
				else if(c != 0) {
					eventQueue_.push(event(event::type::KEYBOARD , static_cast<long>(c)));
				}
			}
			catch(const std::overflow_error& e)
			{
				std::cout << "program not responding: " << e.what() << "\r" << std::endl;
			}
			catch(const std::runtime_error& e)
			{
				std::cout << e.what() << "\r" << std::endl;
				std::terminate();
			}
		}
	}
}

void updateTerminalSize()
{
	struct winsize ws;
	if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws) == 0)
	{
		columns = ws.ws_col;
		rows = ws.ws_row;
	}
	else throw std::runtime_error("unable to fetch window size");
}

void signalHandler(int)
{
	updateTerminalSize();
	eventQueue_.push(event(event::type::RESIZE , 0));
}

// TODO: find a better way to do this 
// a better faster more maintable way 
// flex bison parser??
event readParseEscapeSequence()
{
    char seqBuffer[ansiEscSize_] = {0};
    ssize_t n = read(STDIN_FILENO, seqBuffer, sizeof(seqBuffer));
	
	// handle error hase
	if(n < 0) throw std::runtime_error("was not able to read from stdin , return code: " + std::to_string(n));
	if(n >= ansiEscSize_) throw std::runtime_error("ansi escape sequence overflow");
	
	if(n == 0) {
		return event(event::type::KEYBOARD , static_cast<long>(keyboard::ESCAPE));
	}
	else {
		switch(seqBuffer[0])
		{
			// CONTROL SEQUENCE INTRODUCED (CSI) handles either [ or O else throw
			case '[':
				// numerical and tilda refer to keyboard events:
				if(seqBuffer[1] >= '0' && seqBuffer[1] <= '9' && seqBuffer[2] == '~') {
					switch(seqBuffer[1]) {
						case '1': return event(event::type::KEYBOARD , static_cast<long>(keyboard::HOME));
						case '3': return event(event::type::KEYBOARD , static_cast<long>(keyboard::DELETE));
						case '4': return event(event::type::KEYBOARD , static_cast<long>(keyboard::END));
				        case '5': return event(event::type::KEYBOARD , static_cast<long>(keyboard::PAGE_UP));
						case '6': return event(event::type::KEYBOARD , static_cast<long>(keyboard::PAGE_DOWN));
						case '7': return event(event::type::KEYBOARD , static_cast<long>(keyboard::HOME));
            			case '8': return event(event::type::KEYBOARD , static_cast<long>(keyboard::END));
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
				}
				//alphabeta refer to keyboard events:
				else {
					switch(seqBuffer[1])
					{
						case 'A': return event(event::type::KEYBOARD , static_cast<long>(keyboard::ARROW_UP));
						case 'B': return event(event::type::KEYBOARD , static_cast<long>(keyboard::ARROW_DOWN));
						case 'C': return event(event::type::KEYBOARD , static_cast<long>(keyboard::ARROW_RIGHT));
						case 'D': return event(event::type::KEYBOARD , static_cast<long>(keyboard::ARROW_LEFT));
						case 'H': return event(event::type::KEYBOARD , static_cast<long>(keyboard::HOME));
						case 'F': return event(event::type::KEYBOARD , static_cast<long>(keyboard::END));
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
				}
				break;

			case 'O':
				switch(seqBuffer[1])
				{
					case 'H': return event(event::type::KEYBOARD , static_cast<long>(keyboard::HOME));
					case 'F': return event(event::type::KEYBOARD , static_cast<long>(keyboard::END));
					case 'P': return event(event::type::KEYBOARD , static_cast<long>(keyboard::FN_1));
					case 'Q': return event(event::type::KEYBOARD , static_cast<long>(keyboard::FN_2));
					case 'R': return event(event::type::KEYBOARD , static_cast<long>(keyboard::FN_3));
					case 'S': return event(event::type::KEYBOARD , static_cast<long>(keyboard::FN_4));
					default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
				}
				break;

			default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
		}
	}
}


}
