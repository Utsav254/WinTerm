#include "winTerm.hpp"

winTerm::queue<winTerm::msg, 4> _messageQueue;

void winTerm::postQuitMessage(int returnCode) noexcept
{
	_messageQueue.emplace(message::QUIT , returnCode);
}

void winTerm::postPaintMessage() noexcept
{
	_messageQueue.emplace(message::PAINT , 0);
}

static inline void readParseEscapeSequence(winTerm::msg *m)
{
	constexpr std::size_t seqSize = 16;
	char seqBuffer[seqSize] = {0};
	ssize_t n = read(STDIN_FILENO, seqBuffer, sizeof(seqBuffer));
	// handle error hase
	if(n < 0) throw std::runtime_error("was not able to read from stdin , return code: " + std::to_string(n));
	if(n >= (int)seqSize) throw std::runtime_error("ansi escape sequence overflow");
	
	if(n == 0) {
		new (m) winTerm::msg(winTerm::message::KEYBOARD, winTerm::keyboard::ESCAPE);
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
						case '1': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::HOME);
							break;
						case '3': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::DELETE);
							break;
						case '4': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::END);
							break;
						case '5': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::PAGE_UP);
							break;
						case '6': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::PAGE_DOWN);
							break;
						case '7': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::HOME);
							break;
						case '8': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::END);
							break;
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
				}
				//alphabeta refer to keyboard events:
				else {
					switch(seqBuffer[1])
					{
						case 'A': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::ARROW_UP);
							break;
						case 'B': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::ARROW_DOWN);
							break;
						case 'C': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::ARROW_RIGHT);
							break;
						case 'D': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::ARROW_LEFT);
							break;
						case 'H': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::HOME);
							break;
						case 'F': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::END);
							break;
						default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
					}
				}
				break;

			case 'O':
				switch(seqBuffer[1])
				{
					case 'H': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::HOME);
						break;
					case 'F': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::END);
						break;
					case 'P': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::FN_1);
						break;
					case 'Q': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::FN_2);
						break;
					case 'R': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::FN_3);
						break;
					case 'S': new (m) winTerm::msg(winTerm::message::KEYBOARD , winTerm::keyboard::FN_4);
						break;
					default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
				}
				break;
			default: throw std::runtime_error("unhandled escape sequence: " + std::string(seqBuffer));
		}
	}
}


static inline void readParseAnsiStdIn(winTerm::msg *m) noexcept
{
	char c = 0;
	read(STDIN_FILENO , &c , 1);
	if(c == 27) {
		readParseEscapeSequence(m);
	}
	else if(c != 0) {
		new (m) winTerm::msg(winTerm::message::KEYBOARD, static_cast<winTerm::keyboard>(c));
	}
}

int winTerm::getMessage(msg* m) noexcept
{
	if(_messageQueue.empty()) {
		readParseAnsiStdIn(m);
	}
	else {
		*m = _messageQueue.front();
		_messageQueue.pop();
	}

	if(m->m == message::QUIT) return 0;
	else return 1;
}

void winTerm::postMessage(const msg *m) noexcept
{
	_messageQueue.push(*m);
}
