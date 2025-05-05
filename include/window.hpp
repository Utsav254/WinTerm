#pragma once
#include "registry.hpp"
#include "handle.hpp"
#include "defs.hpp"
#include "rect.hpp"

namespace winTerm
{
	struct msg;

	using wndProcFunc_t = int(*)(msg *m);

	struct window {
	
		window() = default;
		window(const unsigned int x, const unsigned int y,
				const unsigned int columns, const unsigned int rows,
				const wchar_t* windowTitle, wndStyle style,
				wndProcFunc_t wndProc,
				const handle<window> parentWindow,
				void *userData):
			x(x), y(y), columns(columns), rows(rows),
			windowTitle(windowTitle), style(style),
			wndProc(wndProc),
			parentWindow(parentWindow),
			userData(userData) {}

		~window() = default;

		unsigned int x, y;
		unsigned int columns, rows;
		const wchar_t * windowTitle;
		wndStyle style;
		wndProcFunc_t wndProc;
		handle<window> parentWindow;
		void *userData;

		handle<rect> mainRect;
	};

#ifndef WINTERM_DONT_DEFINE_GLOBALS
	extern winTerm::registry<winTerm::window, WINDOW_COUNT_MAX> _windows;
#endif
}
