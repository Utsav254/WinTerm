#include "WinTerm/render/render.hpp"
#include "WinTerm/ansi/stdinReader.hpp"
#include "generics/boundedQueue.hpp"
#include <iostream>

namespace winTerm
{
	// render queue entries ....
	boundedQueue<canvas , numCanvas> renderQueue;
	std::thread renderThread;	
	
	void endRender()
	{
		renderQueue.clear();
	}


	std::unique_ptr<canvas> beginPaint(int rows , int columns) noexcept
	{
		return std::make_unique<canvas>(rows , columns);
	}

	bool endPaint(std::unique_ptr<canvas> canv) noexcept
	{
		return renderQueue.push(std::move(canv));
	}

	void renderCanvas()
	{
		char multiByteChar[4] = {0};
		std::unique_ptr<canvas> canv;
		std::setlocale(LC_ALL, "");

		while(!shouldQuit.load()) {
			
			// fetch render request
			canv = renderQueue.pop(shouldQuit);	
			if(canv == nullptr) continue;

			// tracking variables
			fmt::color currFg = canv->buffer_[0][0].fgColor;
			fmt::color currBg = canv->buffer_[0][0].bgColor;
			fmt::emphasis currEmphasis = canv->buffer_[0][0].style;
			
			// initialise ansi string for render
			std::string ansi = "\x1b[?25l\x1b[H" + std::string(fmt::detail::make_background_color<char>(currBg)) +
											std::string(fmt::detail::make_foreground_color<char>(currFg)) +
 											std::string(fmt::detail::make_emphasis<char>(currEmphasis));


			for(int j = 0 ; j < (int)canv->buffer_.size() ; j++) {
				for (int i = 0 ; i < (int)canv->buffer_[j].size() ; i++) {

					int bytes = wctomb(multiByteChar , canv->buffer_[j][i].character);
					
					if(canv->buffer_[j][i].fgColor != currFg) {
						currFg = canv->buffer_[j][i].fgColor;
						ansi += std::string(fmt::detail::make_foreground_color<char>(currFg));
					}

					if(canv->buffer_[j][i].bgColor != currBg) {
						currBg = canv->buffer_[j][i].bgColor;
						ansi += std::string(fmt::detail::make_background_color<char>(currBg));
					}

					if(canv->buffer_[j][i].style != currEmphasis) {
						currEmphasis = canv->buffer_[j][i].style;
						ansi += std::string(fmt::detail::make_emphasis<char>(currEmphasis));
					}
					
					if(bytes > 0) { ansi.append(multiByteChar , bytes); }

				}
				ansi += "\r\n";
			}
			ansi += "\x1b[?25h\x1b[0m";
			// write entire string to console in one swoop 
			write(STDOUT_FILENO , ansi.c_str() , ansi.size());
		}
	}
}
