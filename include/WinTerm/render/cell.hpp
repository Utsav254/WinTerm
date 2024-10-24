#pragma once
#include <fmt/color.h>

namespace winTerm
{
	struct cell
	{
		wchar_t character;
		fmt::color fgColor;
		fmt::color bgColor;
		fmt::emphasis style;

		
		cell() : 
			character(' ') , 
			fgColor(fmt::color::white) ,
			bgColor(fmt::color::black) ,
			style(static_cast<fmt::emphasis>(0)) {}

		cell(wchar_t c) :
			character(c) ,
			fgColor(fmt::color::white) ,
		   	bgColor(fmt::color::black) ,
		   	style(static_cast<fmt::emphasis>(0)) {}

		cell(wchar_t c , const fmt::color fg , const fmt::color bg , fmt::emphasis style) :
			character(c) ,
			fgColor(fg) ,
			bgColor(bg) ,
			style(style) {}
	};
}

