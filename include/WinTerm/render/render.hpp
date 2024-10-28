#pragma once
#include <memory>
#include <thread>
#include "canvas.hpp"

namespace winTerm
{
	extern std::thread renderThread;
	constexpr std::size_t numCanvas = 64;

	void endRender();

	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	std::unique_ptr<canvas> beginPaint(int rows , int columns) noexcept;
	
	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	bool endPaint(std::unique_ptr<canvas> canv) noexcept;

	// render worker thread
	void renderCanvas();
}
