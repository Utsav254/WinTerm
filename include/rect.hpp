#pragma once
#include "registry.hpp"
#include "defs.hpp"

namespace winTerm
{
	struct rect {
		unsigned int left;
		unsigned int top;
		unsigned int right;
		unsigned int bottom;
		
		rect() = default;
		rect(unsigned int left , unsigned int top , unsigned int right , unsigned int bottom);
		rect& operator &=(const rect& other);
        rect& operator|=(const rect& other);
		bool operator&&(const rect& other) const noexcept;
	};

	extern winTerm::registry<rect, RECT_COUNT_MAX> _rects;
}
