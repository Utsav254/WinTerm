#pragma once
#include <memory>
#include "canvas.hpp"

namespace winTerm
{
	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	std::unique_ptr<canvas> beginPaint() noexcept;
	
	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	void endPaint(std::unique_ptr<canvas> canv) noexcept;

	constexpr std::size_t numCanvas = 64;
}
