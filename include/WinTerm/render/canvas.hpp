#pragma once
#include <vector>
#include "WinTerm/render/misc.hpp"
#include "cell.hpp"
#include <array>

namespace winTerm
{
	class canvas
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
		void drawRect(const rect& rectangle , const fmt::color bg , const borderStyle bs , const bool erase);
		
		// also use the template overload of the drawRect
		// this uses constexpr for evaluating the characters required for each style
		// this is a trial feature which makes use a compile time constexpr to 
		// calculate the boxborders 
		// note: this will bloat your final compiled binary with additional templating
		template <borderStyle style>
		void drawRect(const rect& rectangle , const fmt::color bg , const bool erase) {

			//if(rectangle.bottom > height_ || rectangle.right > width_)
			//	throw std::out_of_range(
			//	fmt::format("rectangle size({:d} , {:d}) out of range" , rectangle.right , rectangle.bottom)
			//	);
			
			// fetch characters at comptime
			constexpr auto borderChars = getBorderCharacters<style>();
			// call normal non template function
			drawRectangleImpl(borderChars , rectangle , bg , erase);
		}

		void getBuffer(std::vector<std::vector<cell>>& buffer) const { buffer = buffer_; }
	
	private:
		// resize the buffer
		// this will ruin the data held in the vector
		void resize(const int newWidth , const int newHeight) noexcept;

		// called by template function 
		// extracted from template function to reduce duplication of code in compiled binary
		void drawRectangleImpl(const std::array<wchar_t, 6>& border_chars , const rect& rectangle,
								const fmt::color bg , const bool erase);

		template <borderStyle style>
		constexpr std::array<wchar_t, 6> getBorderCharacters() {
			if constexpr (style == borderStyle::NONE) {
				return {' ' , ' ' , ' ' , ' ' , ' ' , ' '};
			}
			else if constexpr (style == borderStyle::THIN) {
				return {L'─', L'│', L'┌', L'┐', L'└', L'┘'};
			}
			else if constexpr (style == borderStyle::THICK) {
				return {L'━', L'┃', L'┏', L'┓', L'┗', L'┛'};
			}
			else if constexpr (style == borderStyle::DOUBLE) {
				return {L'═', L'║', L'╔', L'╗', L'╚', L'╝'};
			}
			else {
				return {'?', '?', '?', '?', '?', '?'};
			}
		}

		unsigned int width_ , height_;
		std::vector<std::vector<cell>> buffer_;

		fmt::color background_;
	};
}
