#include "winTerm.hpp"

winTerm::canvas* winTerm::beginPaint(int rows , int columns) noexcept
{
	static canvas canv(rows, columns);
	return &canv;
}

static std::string ansi(512, ' ');

void winTerm::endPaint(canvas* canv) noexcept
{
	ansi.clear();
	ansi += "\x1b[?25l";

	canv->renderStringGenerate(ansi);

	ansi += "\x1b[?25h";
	
	write(STDOUT_FILENO, ansi.c_str(), ansi.size());
}
