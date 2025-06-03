#include <cstring>
#include "canvas.hpp"
#include "defs.hpp"

using namespace winTerm;

static constexpr std::array<wchar_t , 6 * 4>borderChars =
{
	L' ' , L' ' , L' ' , L' ' , L' ' , L' ', // borderStyles::NONE
	L'─' , L'│' , L'┌' , L'┐' , L'└' , L'┘', // borderStyles::THIN
	L'━' , L'┃' , L'┏' , L'┓' , L'┗' , L'┛', // borderStyles::THICK
	L'═' , L'║' , L'╔' , L'╗' , L'╚' , L'╝', // borderStyle::DOUBLE
};

void canvas::clear() noexcept
{
	for(unsigned int j = 0 ; j < height_ ; j++)
		for(unsigned int i = 0 ; i < width_ ; i++)
		{
			buffer_chars_[j][i] = L' ';
			buffer_fg_[j][i] = colour::white;
			buffer_bg_[j][i] = colour::black;
			buffer_emph_[j][i] = emphasis::norm;
		}

}

void canvas::setBackground(const colour col) noexcept
{
	for(std::vector<colour>& row : buffer_bg_) {
		std::memset(row.data(), col, row.size() * sizeof(colour));
	}
}

void canvas::setForeground(const colour col) noexcept
{
	for(std::vector<colour>& row : buffer_fg_) {
		std::memset(row.data(), col, row.size() * sizeof(colour));
	}
}

void canvas::addText(const std::string& str , unsigned int row , unsigned int column , 
				  const colour fg , const colour bg , emphasis style)
{
	if(row > height_ || column > width_) 
		throw std::out_of_range(std::format("addText: index ({:d} , {:d}) out of range" , row , column));
	else {
		for(unsigned int x = 0 ; x < str.size() ; x++) {
			if(x + column >= width_) break;
			buffer_chars_[row][column + x] = str[x];
			buffer_fg_[row][column + x] = fg;
			buffer_bg_[row][column + x] = bg;
			buffer_emph_[row][column + x] = style;
		}
	}
}

void canvas::drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept
{
	unsigned int offset = static_cast<unsigned int>(bs);

	for (unsigned int y = rectangle.top; y <= rectangle.bottom; ++y) {
		if(y >= height_) break;
		for (unsigned int x = rectangle.left; x <= rectangle.right; ++x) {
			if(x >= width_) break;
			if (y == rectangle.top && x == rectangle.left) {
				buffer_chars_[y][x] = borderChars[offset+2]; // Top-left corner
				buffer_bg_[y][x] = bg;
			}
			else if (y == rectangle.top && x == rectangle.right) {
				buffer_chars_[y][x] = borderChars[offset+3]; // Top-right corner
				buffer_bg_[y][x] = bg;
			}
			else if (y == rectangle.bottom && x == rectangle.left) {
				buffer_chars_[y][x] = borderChars[offset+4]; // Bottom-left corner
				buffer_bg_[y][x] = bg;
			}
			else if (y == rectangle.bottom && x == rectangle.right) {
				buffer_chars_[y][x] = borderChars[offset+5]; // Bottom-right corner
				buffer_bg_[y][x] = bg;
			}
			else if (x == rectangle.left || x == rectangle.right) {
				buffer_chars_[y][x] = borderChars[offset+1]; // Vertical border
				buffer_bg_[y][x] = bg;
			}
			else if (y == rectangle.top || y == rectangle.bottom) {
				buffer_chars_[y][x] = borderChars[offset+0]; // Horizontal border
				buffer_bg_[y][x] = bg;
			}

			else {
				if(erase) {
					buffer_chars_[y][x] = L' '; // Background character
					buffer_bg_[y][x] = bg;
				}
				else {
					buffer_bg_[y][x] = bg;
				}
			}
		}
	}
}

void canvas::setBorder(const borderStyle bs, const colour fg) noexcept
{
	unsigned int offset = static_cast<unsigned int>(bs);
	// corners 
	buffer_chars_[0][0] = borderChars[offset+2];
	buffer_chars_[0][width_ - 1] = borderChars[offset+3];
	buffer_chars_[height_ - 1][0] = borderChars[offset+4];
	buffer_chars_[height_ - 1][width_ - 1] = borderChars[offset+5];
	
	buffer_fg_[0][0] = fg; 
	buffer_fg_[0][width_ - 1] = fg; 
	buffer_fg_[height_ - 1][0] = fg; 
	buffer_fg_[height_ - 1][width_ - 1] = fg; 

	// horizontal borders
	for(unsigned int i = 1 ; i < width_ - 1 ; i++) {
		buffer_chars_[0][i] = borderChars[offset];
		buffer_fg_[0][i] = fg; 

		buffer_chars_[height_ - 1][i] = borderChars[offset];
		buffer_fg_[height_ - 1][i] = fg;
	}
	
	// vertical borders
	for(unsigned int j = 1 ; j < height_ - 1 ; j++) {
		buffer_chars_[j][0] = borderChars[offset+1];
		buffer_fg_[j][0] = fg; 

		buffer_chars_[j][width_ - 1] = borderChars[offset+1];
		buffer_fg_[j][width_ - 1] = fg; 
	}
}

void canvas::setPosition(const unsigned int x, const unsigned int y)
{
	x_ = x;
	y_ = y;
}


void canvas::renderStringGenerate(std::string& out) const noexcept
{
	char multiByteChar[4] = {0};

	colour currFg = buffer_fg_[0][0];
	colour currBg = buffer_bg_[0][0];
	emphasis currEmphasis = buffer_emph_[0][0];

	// initialise ansi string for render
	out += std::move(std::format("\x1b[{:d};{:d}H\x1b[48;2;{:d};{:d};{:d}m\x1b[38;2;{:d};{:d};{:d}m\x1b[{:c}m",
					(y_), (x_),
					(currBg & colour::red) >> 16, (currBg & colour::green) >> 8 , (currBg & colour::blue),
					(currFg & colour::red) >> 16, (currFg & colour::green) >> 8 , (currFg & colour::blue),
					attribute_codes[currEmphasis]
	));

	for(unsigned int j = 0 ; j < height_ ; j++) {
		for (unsigned int i = 0 ; i < width_ ; i++) {

			const int bytes = wctomb(multiByteChar , buffer_chars_[j][i]);
			
			if(buffer_fg_[j][i] != currFg) {
				currFg = buffer_fg_[j][i];
				out += std::move(std::format("\x1b[38;2;{:d};{:d};{:d}m",
					(currFg & colour::red) >> 16, (currFg & colour::green) >> 8 , (currFg & colour::blue)
				));
			}

			if(buffer_bg_[j][i] != currBg) {
				currBg = buffer_bg_[j][i];
				out += std::move(std::format("\x1b[48;2;{:d};{:d};{:d}m",
					(currBg & colour::red) >> 16, (currBg & colour::green) >> 8 , (currBg & colour::blue)
				));
			}

			if(buffer_emph_[j][i] != currEmphasis) {
				currEmphasis = buffer_emph_[j][i];
				out += std::move(std::format("\x1b[{:c}m",attribute_codes[currEmphasis]));
			}
			
			if(bytes > 0) { out.append(multiByteChar , bytes); }

		}
		out += renderStrGenResetSeq;
	}
}

