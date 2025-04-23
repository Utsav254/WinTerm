#include "WinTerm/winTerm.hpp"

handle<winTerm::canvas> winTerm::beginPaint(int rows , int columns) noexcept
{
	handle<canvas> h = _renderQueue.handleGet(rows , columns);
	return h;
}

void winTerm::endPaint(handle<canvas> canv) noexcept
{
	_renderQueue.push(std::move(canv));
}

void winTerm::renderCanvas() noexcept
{
	handle<canvas> canv;
	
	while((canv = _renderQueue.pop())->message_ != canvas::canvMsg::END) {

		std::string ansi = "\x1b[?25l";
		canv->renderStringGenerate(ansi);
		ansi += "\x1b[?25h";
		
		write(STDOUT_FILENO, ansi.c_str(), ansi.size());
	}
}
