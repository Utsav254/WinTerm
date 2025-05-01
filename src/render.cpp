#include "winTerm.hpp"

winTerm::canvas* winTerm::beginPaint(int rows , int columns) noexcept
{
	static canvas canv(rows, columns);
	canv.resize(rows, columns);
	return &canv;
}

void winTerm::endPaint(canvas* canv) noexcept
{
	std::string ansi = "\x1b[?25l";
	canv->renderStringGenerate(ansi);
	ansi += "\x1b[?25h";
	
	write(STDOUT_FILENO, ansi.c_str(), ansi.size());
}
