#include "winTerm.hpp"
using namespace winTerm;

cell::cell(): 
	character(' '), 
	fgColor(colour::white),
	bgColor(colour::black),
	emph(static_cast<emphasis>(0))
{}

cell::cell(wchar_t c):
	character(c),
	fgColor(colour::white),
	bgColor(colour::black),
	emph(static_cast<emphasis>(0))
{}

cell::cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph):
	character(c),
	fgColor(fg),
	bgColor(bg),
	emph(emph)
{}
