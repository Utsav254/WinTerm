#pragma once
#include "canvas.hpp"
#include "handle.hpp"
#include "defs.hpp"

#define WINTERM_DONT_DEFINE_GLOBALS
#include "msg.hpp"
#include "window.hpp"
#undef WINTERM_DONT_DEFINE_GLOBALS

namespace winTerm {

	// will open new terminal buffer and terminal will enter raw mode
	// only call once at the start of the program
	int initialise();

	// cleanup entire winTerm context
	// will close alternative terminal buffer and restore previous terminal settings
	int destroy();

	// use to bind message to quitting program
	// set the return Code if required
	// if unable to post, will force push by popping extra event and pushing
	void postQuitMessage(int returnCode) noexcept;

	// use to bind message to a painting operation
	// return true on success and false on failure
	void postPaintMessage() noexcept;
	
	// use to get events from event queue
	// return value is 0 if QUIT message
	// positive if no error
	// negative if error
	int getMessage(msg *m) noexcept;

	// use to dispatch message to relevant window
	// calls the relevant windows message handler
	// return value is the one returned by message handler
	int dispatchMessage(msg *m) noexcept;

	// post a message to the message queue
	// silent on failure (unlikely to fail)
	void postMessage(const msg *m) noexcept;

	// send message and get message response (int)
	// sent to relevant message handling function
	int sendMessage(const msg* m) noexcept;

	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	canvas* beginPaint(int rows , int columns) noexcept;

	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	void endPaint(canvas* canv) noexcept;

	handle<window> createWindow(const unsigned int x, const unsigned int y,
								const unsigned int columns, const unsigned int rows,
								const wchar_t* windowTitle, wndStyle style,
								wndProcFunc_t wndProc,
								const handle<window> parentWindow,
								void *userData) noexcept;

	void destroyWindow(handle<window> wnd) noexcept;
}
