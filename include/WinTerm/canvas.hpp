#pragma once
#include <format>
#include <vector>
#include <array>
#include "WinTerm/styles.hpp"
#include "WinTerm/rect.hpp"

namespace winTermNameSpace {

	struct cell
	{
		wchar_t character;
		colour fgColor;
		colour bgColor;
		emphasis emph;

		
		cell() : 
			character(' ') , 
			fgColor(colour::white),
			bgColor(colour::black),
			emph(static_cast<emphasis>(0)){}

		cell(wchar_t c) :
			character(c) ,
			fgColor(colour::white) ,
			bgColor(colour::black) ,
			emph(static_cast<emphasis>(0)){}

		cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph) :
			character(c) ,
			fgColor(fg) ,
			bgColor(bg) ,
			emph(emph) {}
	};

	class canvas final {
	public:

		enum class canvMsg
		{
			OPTIMISE,
			FORCE_RERENDER,
			END,
		};

	public:
		canvas(
			const unsigned int width, const unsigned int height, 
			const cell& cl = {L' ', colour::white, colour::black, emphasis::norm},
			const canvMsg msg = canvMsg::OPTIMISE):

			message_(msg),
			width_(width) , height_(height),
			buffer_(height , std::vector<cell>(width , cl)),
			renderStrGenResetSeq(std::format("\x1b[1B\x1b[{:d}D", width_))
		{
			if((width < 1 || height < 1) && msg != canvMsg::END)
				throw std::out_of_range(std::format("invalid size({:d} , {:d})" , width , height));
		}

		~canvas() = default;

		canvas(const canvas& ) = delete;
		canvas& operator=(const canvas&) = delete;
		
		// fetch cell reference at row or column
		// throw std::out_of_range if not in buffer
		cell& at(const unsigned int row , const unsigned int column);

		// clear the buffer completely with default cell
		void clear() noexcept;
		// clear the buffer and put a specified cell in place
		void clear(const cell& cl) noexcept;
		
		// setBackGround of entire buffer
		// will not modify character content of buffer
		void setBackground(const colour col) noexcept;
		inline colour getBackground() const noexcept { return background_; }

		// add text to a window
		// throw std::out_of_range if given string is completely out of range
		// no automatic wrapping will simply cut string off at the end of the buffer
		void addText(const std::string& str , unsigned int row , unsigned int column , 
			   const colour fg , const colour bg , emphasis emph);

		// draw a rectangle at given location (window coordinates)
		// throw std::out_of_range if given rect is completely out of range
		// if escaping buffer, will cutoff at border
		// set erase true if the rectangle should erase existing characters in specified area
		void drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept;
		
		// set the border of the window buffer with given border style
		void setBorder(const borderStyle bs) noexcept;


		//
		void getBuffer(std::vector<std::vector<cell>>& buffer) const { buffer = buffer_; }
		void updateRenderScheme(canvMsg in) noexcept { message_ = in; }


		
		canvMsg message_;
		//generate render string based on render schema
		void renderStringGenerate(std::string& out) const noexcept;

	private:
		// resize the buffer
		// this will ruin the data held in the vector
		void resize(const int newWidth , const int newHeight) noexcept;


		unsigned int width_ , height_;
		std::vector<std::vector<cell>> buffer_;
		colour background_;

		const std::string renderStrGenResetSeq;

		static constexpr std::array<wchar_t , 6 * 4>borderChars =
		{
			L' ' , L' ' , L' ' , L' ' , L' ' , L' ', // borderStyles::NONE

			L'─' , L'│' , L'┌' , L'┐' , L'└' , L'┘', // borderStyles::THIN

			L'━' , L'┃' , L'┏' , L'┓' , L'┗' , L'┛', // borderStyles::THICK

			L'═' , L'║' , L'╔' , L'╗' , L'╚' , L'╝', // borderStyle::DOUBLE
		};
	};
}
