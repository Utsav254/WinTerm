#include "WinTerm/render/canvas.hpp"
#include <stdexcept>
#include <array>

namespace winTerm
{
	void canvas::resize(const int newWidth , const int newHeight) noexcept
	{
		buffer_.resize(newHeight , std::vector<cell>(newWidth , cell()));	
	}

	cell& canvas::at(const unsigned int row , const unsigned int column)
	{
		if(row < height_ && column < width_) {
			return buffer_[row][column];
		}
		else {
			throw std::out_of_range(fmt::format("at: index ({:d} , {:d}) out of range" , row , column));
		}
	}

	void canvas::clear() noexcept
	{
		for(std::vector<cell>& row : buffer_) {
			for(cell& elem : row) {
				elem.character = L' ';
			}
		}
	}

	void canvas::clear(const cell& cl) noexcept
	{
		for(std::vector<cell>& row : buffer_) {
			for(cell& elem : row) {
				elem = cl;
			}
		}
	}

	void canvas::setBackground(const fmt::color col) noexcept
	{
		for(std::vector<cell>& row : buffer_) {
			for(cell& elem : row) {
				elem.bgColor = col;
			}
		}
		background_ = col;
	}

	void canvas::addText(const std::string& str , unsigned int row , unsigned int column , 
					  const fmt::color fg , const fmt::color bg , fmt::emphasis style)
	{
		if(row > height_ || column > width_) 
			throw std::out_of_range(fmt::format("addText: index ({:d} , {:d}) out of range" , row , column));
		else {
			for(unsigned int x = 0 ; x < str.size() ; x++) {
				if(x + column >= width_) break;
				buffer_[row][column + x] = cell(str[x] , fg , bg , style);
			}
		}
	}

	void canvas::drawRect(const rect& rectangle , const fmt::color bg , const borderStyle bs , const bool erase) noexcept
	{
		unsigned int offset = static_cast<unsigned int>(bs);

		for (unsigned int y = rectangle.top; y <= rectangle.bottom; ++y) {
			if(y >= height_) break;
			for (unsigned int x = rectangle.left; x <= rectangle.right; ++x) {
				if(x >= width_) break;
				if (y == rectangle.top && x == rectangle.left) {
					buffer_[y][x].character = borderChars[offset+2]; // Top-left corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.top && x == rectangle.right) {
					buffer_[y][x].character = borderChars[offset+3]; // Top-right corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.bottom && x == rectangle.left) {
					buffer_[y][x].character = borderChars[offset+4]; // Bottom-left corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.bottom && x == rectangle.right) {
					buffer_[y][x].character = borderChars[offset+5]; // Bottom-right corner
					buffer_[y][x].bgColor = bg;
				}
				else if (x == rectangle.left || x == rectangle.right) {
					buffer_[y][x].character = borderChars[offset+1]; // Vertical border
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.top || y == rectangle.bottom) {
					buffer_[y][x].character = borderChars[offset+0]; // Horizontal border
					buffer_[y][x].bgColor = bg;
				}

				else {
					if(erase) {
						buffer_[y][x].character = L' '; // Background character
						buffer_[y][x].bgColor = bg;
					}
					else {
						buffer_[y][x].bgColor = bg;
					}
				}
			}
		}
    }

	void canvas::setBorder(const borderStyle bs) noexcept
	{
		unsigned int offset = static_cast<unsigned int>(bs);
		// corners 
		buffer_[0][0].character = borderChars[offset+2];
		buffer_[0][width_ - 1].character = borderChars[offset+3];
		buffer_[height_ - 1][0].character = borderChars[offset+4];
		buffer_[height_ - 1][width_ - 1].character = borderChars[offset+5];
		
		// horizontal borders
		for(unsigned int i = 1 ; i < width_ - 1 ; i++) {
			buffer_[0][i].character = borderChars[offset];
 			buffer_[height_ - 1][i].character = borderChars[offset];
		}
		
		// vertical borders
		for(unsigned int j = 1 ; j < height_ - 1 ; j++) {
			buffer_[j][0].character = borderChars[offset+1];
			buffer_[j][width_ - 1].character = borderChars[offset+1];
		}
    }


	void canvas::renderStringGenerate(std::string& out) const noexcept
	{
		char multiByteChar[4] = {0};

		fmt::color currFg = buffer_[0][0].fgColor;
		fmt::color currBg = buffer_[0][0].bgColor;
		fmt::emphasis currEmphasis = buffer_[0][0].style;

		// initialise ansi string for render
		out += std::string(fmt::detail::make_background_color<char>(currBg)) +
										std::string(fmt::detail::make_foreground_color<char>(currFg)) +
										std::string(fmt::detail::make_emphasis<char>(currEmphasis));


		for(int j = 0 ; j < (int)buffer_.size() ; j++) {
			for (int i = 0 ; i < (int)buffer_[j].size() ; i++) {

				int bytes = wctomb(multiByteChar , buffer_[j][i].character);
				
				if(buffer_[j][i].fgColor != currFg) {
					currFg = buffer_[j][i].fgColor;
					out += std::string(fmt::detail::make_foreground_color<char>(currFg));
				}

				if(buffer_[j][i].bgColor != currBg) {
					currBg = buffer_[j][i].bgColor;
					out += std::string(fmt::detail::make_background_color<char>(currBg));
				}

				if(buffer_[j][i].style != currEmphasis) {
					currEmphasis = buffer_[j][i].style;
					out += std::string(fmt::detail::make_emphasis<char>(currEmphasis));
				}
				
				if(bytes > 0) { out.append(multiByteChar , bytes); }

			}
			out += renderStrGenResetSeq;
		}
	}
}
