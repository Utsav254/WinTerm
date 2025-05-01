#include "winTerm.hpp"
using namespace winTerm;

rect::rect(unsigned int left , unsigned int top , unsigned int right , unsigned int bottom):
	left(left) , top(top) , right(right) , bottom(bottom)
{
	if(right < left || bottom < top) {

		#ifdef _DEBUG
		throw std::out_of_range
		(
			std::format("rectangle bottom/right less than top/left({:d}, {:d}, {:d}, {:d})", left, top, right, bottom)
		);
		#endif

		right = left;
		bottom = top;
	}
}

rect& rect::operator &=(const rect& other)
{
	left = std::max(left, other.left);
	top = std::max(top, other.top);
	right = std::min(right, other.right);
	bottom = std::min(bottom, other.bottom);

	if (right < left || bottom < top) {
		left = top = right = bottom = 0;
	}

	return *this;
}

rect& rect::operator|=(const rect& other)
{
	left = std::min(left, other.left);
	top = std::min(top, other.top);
	right = std::max(right, other.right);
	bottom = std::max(bottom, other.bottom);

	return *this;
}
