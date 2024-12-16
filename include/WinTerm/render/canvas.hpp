#pragma once
#include <vector>
#include "WinTerm/render/misc.hpp"
#include "cell.hpp"
#include <array>


namespace winTerm
{
	class canvas final
	{
	public:
		canvas(const unsigned int width , const unsigned int height) :
			width_(width) , height_(height),
			buffer_(height , std::vector<cell>(width , cell()))
		{
			if(width < 1 || height < 1) throw std::out_of_range(fmt::format("invalid size({:d} , {:d})" , width , height));
		}

		canvas(const int width , const int height , const cell& cl) :
			width_(width) , height_(height),
			buffer_(height , std::vector<cell>(width , cl))
		{
			if(width < 1 || height < 1) throw std::out_of_range(fmt::format("invalid size({:d} , {:d})" , width , height));
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
		void setBackground(const fmt::color col) noexcept;
		inline fmt::color getBackground() const noexcept { return background_; }
	
		// add text to a window
		// throw std::out_of_range if given string is completely out of range
		// no automatic wrapping will simply cut string off at the end of the buffer
		void addText(const std::string& str , unsigned int row , unsigned int column , 
			   const fmt::color fg , const fmt::color bg , fmt::emphasis style);

		// draw a rectangle at given location (window coordinates)
		// throw std::out_of_range if given rect is completely out of range
		// if escaping buffer, will cutoff at border
		// set erase true if the rectangle should erase existing characters in specified area
		void drawRect(const rect& rectangle , const fmt::color bg , const borderStyle bs , const bool erase) noexcept;
		
		// set the border of the window buffer with given border style
		void setBorder(const borderStyle bs) noexcept;

		void getBuffer(std::vector<std::vector<cell>>& buffer) const { buffer = buffer_; }

		friend void renderCanvas();
	
	private:
		// resize the buffer
		// this will ruin the data held in the vector
		void resize(const int newWidth , const int newHeight) noexcept;

		unsigned int width_ , height_;
		std::vector<std::vector<cell>> buffer_;
		fmt::color background_;

		static constexpr std::array<wchar_t , 6 * 4>borderChars =
		{
			L' ' , L' ' , L' ' , L' ' , L' ' , L' ', // borderStyles::NONE

			L'─' , L'│' , L'┌' , L'┐' , L'└' , L'┘', // borderStyles::THIN

			L'━' , L'┃' , L'┏' , L'┓' , L'┗' , L'┛', // borderStyles::THICK

			L'═' , L'║' , L'╔' , L'╗' , L'╚' , L'╝', // borderStyle::DOUBLE
		};
	};
}
