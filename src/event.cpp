#include "WinTerm/winTerm.hpp"

void winTerm::postQuitMessage(int returnCode)
{
	_shouldQuit.store(true);
	_renderQueue.emplace(0, 0,0,0, cell(), canvas::canvMsg::END);
	_messageQueue.emplace(message::QUIT , returnCode);
}

void winTerm::postPaintMessage()
{
	_messageQueue.emplace(message::PAINT , 0);
}

int winTerm::getMessage(handle<msg>& msg) noexcept
{
	msg = _messageQueue.pop();

	if(msg == nullptr) return -1;
	else if(msg->m == message::QUIT) return 0;
	else return 1;
}
