#pragma once
#include <stdexcept>
#include <format>
#include <vector>

#include "rect.hpp"
#include "defs.hpp"

namespace winTerm
{
	struct cell {
		wchar_t character;
		colour fgColor;
		colour bgColor;
		emphasis emph;

		cell();
		cell(wchar_t c);
		cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph);
	};

	enum class canvMsg {
		OPTIMISE,
		FORCE_RERENDER,
		END,
	};

	struct canvas {
	public:
		canvas(
			const unsigned int width, const unsigned int height,
			const unsigned int x = 0, const unsigned int y = 0,
			const cell& cl = {L' ', colour::white, colour::black, emphasis::norm},
			const canvMsg msg = canvMsg::OPTIMISE):

			width_(width) , height_(height),
			x_(x), y_(y),
			buffer_(height , std::vector<cell>(width , cl)),
			renderStrGenResetSeq(std::format("\x1b[1B\x1b[{:d}D", width_)),
			message_(msg)
		{
			if((width < 1 || height < 1) && msg != canvMsg::END)
				throw std::out_of_range(std::format("invalid size({:d} , {:d})" , width , height));
		}

		~canvas() = default;

		canvas(const canvas& ) = delete;
		canvas& operator=(const canvas&) = delete;

		void resize(const int newWidth , const int newHeight) noexcept;
		
		cell& at(const unsigned int row , const unsigned int column);

		void clear() noexcept;
		void clear(const cell& cl) noexcept;
		
		void setBackground(const colour col) noexcept;
		inline colour getBackground() const noexcept { return background_; }

		void addText(const std::string& str , unsigned int row , unsigned int column , 
			   const colour fg , const colour bg , emphasis emph);

		void drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept;
		
		void setBorder(const borderStyle bs) noexcept;

		void getBuffer(std::vector<std::vector<cell>>& buffer) const { buffer = buffer_; }
		void updateRenderScheme(canvMsg in) noexcept { message_ = in; }

		void setPosition(const unsigned int x, const unsigned int y);
		
		void renderStringGenerate(std::string& out) const noexcept;

		inline canvMsg getMessage() const noexcept {return message_;}

	private:
		unsigned int width_ , height_;
		unsigned int x_, y_;
		std::vector<std::vector<cell>> buffer_;
		colour background_;

		const std::string renderStrGenResetSeq;

		canvMsg message_;
	};
	
}
