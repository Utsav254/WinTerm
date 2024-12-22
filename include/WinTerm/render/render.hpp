#pragma once
#include "generics/channel.hpp"
#include <thread>
#include "canvas.hpp"

namespace winTerm
{

	extern std::thread renderThread;
	constexpr std::size_t numCanvas = 64;
	extern channel<canvas , numCanvas> renderQueue;

	void endRender();

	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	handle<canvas> beginPaint(int rows , int columns) noexcept;
	
	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	void endPaint(handle<canvas> canv) noexcept;

	// render worker thread
	void renderCanvas();
}
