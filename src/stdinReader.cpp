#include "WinTerm/winTerm.hpp"


void winTerm::stdinReader() noexcept
{
	while(!_shouldQuit.load())
	{
		char c = 0;
		read(STDIN_FILENO , &c , 1);
		if(c == 27) {
			readParseEscapeSequence();
		}
		else if(c != 0) {
			_messageQueue.emplace(message::KEYBOARD , static_cast<keyboard>(c));
		}
	}
}


// TODO: find a better way to do this 
// a better faster more maintable way 
// flex bison parser??
// throws for unhandled escape sequences
void winTerm::readParseEscapeSequence()
{
	char seqBuffer[queueSize] = {0};
	ssize_t n = read(STDIN_FILENO, seqBuffer, sizeof(seqBuffer));
	// handle error hase
	if(n < 0) throw std::runtime_error("was not able to read from stdin , return code: " + std::to_string(n));
	if(n >= (int)queueSize) throw std::runtime_error("ansi escape sequence overflow");
	
	if(n == 0) {
		_messageQueue.emplace(message::KEYBOARD , keyboard::ESCAPE);
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
						case '1': _messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
							break;
						case '3': _messageQueue.emplace(message::KEYBOARD , keyboard::DELETE);
							break;
						case '4': _messageQueue.emplace(message::KEYBOARD , keyboard::END);
							break;
						case '5': _messageQueue.emplace(message::KEYBOARD , keyboard::PAGE_UP);
							break;
						case '6': _messageQueue.emplace(message::KEYBOARD , keyboard::PAGE_DOWN);
							break;
						case '7': _messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
							break;
						case '8': _messageQueue.emplace(message::KEYBOARD , keyboard::END);
							break;
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
				}
				//alphabeta refer to keyboard events:
				else {
					switch(seqBuffer[1])
					{
						case 'A': _messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_UP);
							break;
						case 'B': _messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_DOWN);
							break;
						case 'C': _messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_RIGHT);
							break;
						case 'D': _messageQueue.emplace(message::KEYBOARD , keyboard::ARROW_LEFT);
							break;
						case 'H': _messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
							break;
						case 'F': _messageQueue.emplace(message::KEYBOARD , keyboard::END);
							break;
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
				}
				break;

			case 'O':
				switch(seqBuffer[1])
				{
					case 'H': _messageQueue.emplace(message::KEYBOARD , keyboard::HOME);
						break;
					case 'F': _messageQueue.emplace(message::KEYBOARD , keyboard::END);
						break;
					case 'P': _messageQueue.emplace(message::KEYBOARD , keyboard::FN_1);
						break;
					case 'Q': _messageQueue.emplace(message::KEYBOARD , keyboard::FN_2);
						break;
					case 'R': _messageQueue.emplace(message::KEYBOARD , keyboard::FN_3);
						break;
					case 'S': _messageQueue.emplace(message::KEYBOARD , keyboard::FN_4);
						break;
					default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
				}
				break;
			default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
		}
	}
}
