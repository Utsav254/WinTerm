#pragma once
#include <cstddef>
#include <thread>
#include <termios.h>

#include "generics/channel.hpp"
#include "WinTerm/message.hpp"
#include "WinTerm/canvas.hpp"
#include "WinTerm/styles.hpp"

class winTerm
{
public:
	static constexpr std::size_t queueSize = 64;

	using colour = winTermNameSpace::colour;
	using borderStyle = winTermNameSpace::borderStyle;
	using emphasis = winTermNameSpace::emphasis;
	using cell = winTermNameSpace::cell;
	using rect = winTermNameSpace::rect;
	using canvas = winTermNameSpace::canvas;
	using msg = winTermNameSpace::msg;
	using message = winTermNameSpace::message;
	using keyboard = winTermNameSpace::keyboard;

public:
	// initilise winTerm context
	// will open new terminal buffer and terminal will enter raw mode
	// only call once at the start of the program
	static int initialise();

	// cleanup entire winTerm context
	// will close alternative terminal buffer and restore previous terminal settings
	static int destroy();

public:

	// use to bind message to quitting program
	// set the return Code if required
	// if unable to post, will force push by popping extra event and pushing
	static void postQuitMessage(int returnCode);

	// use to bind message to a painting operation
	// return true on success and false on failure
	static void postPaintMessage();
	
	// use to get events from event queue
	// return value is 0 if QUIT message
	// positive if no error
	// negative if error
	static int getMessage(handle<msg>& msg) noexcept;

public:
	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	static handle<canvas> beginPaint(int rows , int columns) noexcept;
	
	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	static void endPaint(handle<canvas> canv) noexcept;



private:
	winTerm() = default;
	~winTerm() = default;

	winTerm(const winTerm&) = delete;
	winTerm& operator=(const winTerm&) = delete;


private:
	// shared important variables
	static inline std::atomic <bool> _shouldQuit;
	static inline int _columns , _rows; // size of full terminal window

private:

	static inline struct termios _origTermios;
	static inline char *_oldLocale;

	// stdin escape sequence parsing
	static inline std::thread _stdinReaderThread;
	static inline channel<msg , queueSize> _messageQueue;
	static void stdinReader() noexcept;
	static void readParseEscapeSequence();

	// rendering back end
	static inline std::thread _renderThread;
	static inline channel<canvas , queueSize> _renderQueue;
	static void renderCanvas() noexcept;
};
