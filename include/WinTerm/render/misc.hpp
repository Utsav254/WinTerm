#pragma once
#include <fmt/format.h>
#include <stdexcept>

namespace winTerm
{
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

	enum class borderStyle{
		NONE,
		THICK,
		THIN,
		DOUBLE
	};

}
