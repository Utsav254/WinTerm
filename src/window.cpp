#include "winTerm.hpp"
#include "registry.hpp"

using namespace winTerm;

namespace winTerm
{
	winTerm::registry<winTerm::window, WINDOW_COUNT_MAX> _windows;
	winTerm::registry<winTerm::rect, RECT_COUNT_MAX> _rects;
}

handle<winTerm::window> winTerm::createWindow(const unsigned int x, const unsigned int y,
								const unsigned int columns, const unsigned int rows,
								const wchar_t* windowTitle, wndStyle style,
								wndProcFunc_t wndProc,
								const handle<winTerm::window> parentWindow,
								void *userData) noexcept
{
	// check x, y, rows, columns to ensure it fits in the terminal
	rect temp(x, y, x + columns, y + rows);
	
	for (const rect & r : _rects) {
		if(r && temp) {
			return nullptr;
		}
	}
	handle<rect> hRect = _rects.allocate(x, y, x + columns, y + rows);

	// allocate window
	handle<window> hWnd = _windows.allocate(x, y, columns, rows, windowTitle, style, wndProc, parentWindow, userData);
	_windows.get(hWnd).mainRect = hRect;


	// send CREATE, SIZE, PAINT messages
	//
	return hWnd;
}

void winTerm::destroyWindow(handle<winTerm::window> wnd) noexcept
{
	_rects.deallocate(_windows.get(wnd).mainRect);
	_windows.deallocate(wnd);
	return;
}
