#include "WinTerm/render/render.hpp"
#include "WinTerm/ansi/stdinReader.hpp"
#include "generics/channel.hpp"
#include <pthread.h>

namespace winTerm
{
	// render queue entries ....
	channel<canvas , numCanvas> renderQueue;
	std::thread renderThread;	
	
	handle<canvas> beginPaint(int rows , int columns) noexcept
	{
		handle<canvas> h = renderQueue.handleGet(rows , columns);
		return h;
	}

	void endPaint(handle<canvas> canv) noexcept
	{
		renderQueue.push(std::move(canv));
	}

	void renderCanvas()
	{
		pthread_setname_np(pthread_self(), "render");
		handle<canvas> canv;
		
		while((canv = renderQueue.pop())->message_ != canvas::canvasMessage::END) {

			std::string ansi = "\x1b[?25l\x1b[H";
			canv->renderStringGenerate(ansi);
			ansi += "\x1b[?25h\x1b[0m";
			
			write(STDOUT_FILENO, ansi.c_str(), ansi.size());
		}
	}
}
