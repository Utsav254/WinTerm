#pragma once
#include "WinTerm/message.hpp"

namespace winTermNameSpace {

	using LRESULT = long;

	class window
	{
	public:
		window();
		virtual ~window();
		
		virtual LRESULT wndProc(msg m) = 0;

	private:
		unsigned int width_;
		unsigned int height_;
		unsigned int x_;
		unsigned int y_;

		window *childTopMost_;
		window *sibling_;
		window *parent_;
	};
}
