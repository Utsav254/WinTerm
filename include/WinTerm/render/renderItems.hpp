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

	enum class borderStyle{
		NONE = 0,
		THICK = 6,
		THIN = 12,
		DOUBLE = 18
	};

	struct rect
	{
		unsigned int left;
		unsigned int top;
		unsigned int right;
		unsigned int bottom;

		rect(unsigned int left , unsigned int top , unsigned int right , unsigned int bottom) :
			left(left) , top(top) , right(right) , bottom(bottom)
		{
			if(right < left || bottom < top) throw std::out_of_range(
				fmt::format("rectangle bottom/right less than top/left({:d} , {:d} , {:d} , {:d})" , left , top , right , bottom)
			);
		}
	};
}

