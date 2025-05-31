#include "canvas.hpp"

using namespace winTerm;

cell::cell(): 
	character(' '), 
	fgColor(colour::white),
	bgColor(colour::black),
	emph(static_cast<emphasis>(0))
{}

cell::cell(wchar_t c):
	character(c),
	fgColor(colour::white),
	bgColor(colour::black),
	emph(static_cast<emphasis>(0))
{}

cell::cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph):
	character(c),
	fgColor(fg),
	bgColor(bg),
	emph(emph)
{}

static constexpr std::array<wchar_t , 6 * 4>borderChars =
{
	L' ' , L' ' , L' ' , L' ' , L' ' , L' ', // borderStyles::NONE
	L'─' , L'│' , L'┌' , L'┐' , L'└' , L'┘', // borderStyles::THIN
	L'━' , L'┃' , L'┏' , L'┓' , L'┗' , L'┛', // borderStyles::THICK
	L'═' , L'║' , L'╔' , L'╗' , L'╚' , L'╝', // borderStyle::DOUBLE
};

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
		throw std::out_of_range(std::format("at: index ({:d} , {:d}) out of range" , row , column));
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

void canvas::setBackground(const colour col) noexcept
{
	for(std::vector<cell>& row : buffer_) {
		for(cell& elem : row) {
			elem.bgColor = col;
		}
	}
	background_ = col;
}

void canvas::addText(const std::string& str , unsigned int row , unsigned int column , 
				  const colour fg , const colour bg , emphasis style)
{
	if(row > height_ || column > width_) 
		throw std::out_of_range(std::format("addText: index ({:d} , {:d}) out of range" , row , column));
	else {
		for(unsigned int x = 0 ; x < str.size() ; x++) {
			if(x + column >= width_) break;
			buffer_[row][column + x] = cell(str[x] , fg , bg , style);
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

void canvas::setPosition(const unsigned int x, const unsigned int y)
{
	x_ = x;
	y_ = y;
}


void canvas::renderStringGenerate(std::string& out) const noexcept
{
	char multiByteChar[4] = {0};

	colour currFg = buffer_[0][0].fgColor;
	colour currBg = buffer_[0][0].bgColor;
	emphasis currEmphasis = buffer_[0][0].emph;

	// initialise ansi string for render
	out += std::move(std::format("\x1b[{:d};{:d}H\x1b[48;2;{:d};{:d};{:d}m\x1b[38;2;{:d};{:d};{:d}m\x1b[{:c}m",
					(y_), (x_),
					(currBg & colour::red) >> 16, (currBg & colour::green) >> 8 , (currBg & colour::blue),
					(currFg & colour::red) >> 16, (currFg & colour::green) >> 8 , (currFg & colour::blue),
					attribute_codes[currEmphasis]
				));
	for(int j = 0 ; j < (int)buffer_.size() ; j++) {
		for (int i = 0 ; i < (int)buffer_[j].size() ; i++) {

			const int bytes = wctomb(multiByteChar , buffer_[j][i].character);
			
			if(buffer_[j][i].fgColor != currFg) {
				currFg = buffer_[j][i].fgColor;
				out += std::move(std::format("\x1b[38;2;{:d};{:d};{:d}m",
					(currFg & colour::red) >> 16, (currFg & colour::green) >> 8 , (currFg & colour::blue)
				));
			}

			if(buffer_[j][i].bgColor != currBg) {
				currBg = buffer_[j][i].bgColor;
				out += std::move(std::format("\x1b[48;2;{:d};{:d};{:d}m",
					(currBg & colour::red) >> 16, (currBg & colour::green) >> 8 , (currBg & colour::blue)
				));
			}

			if(buffer_[j][i].emph != currEmphasis) {
				currEmphasis = buffer_[j][i].emph;
				out += std::move(std::format("\x1b[{:c}m",attribute_codes[currEmphasis]));
			}
			
			if(bytes > 0) { out.append(multiByteChar , bytes); }

		}
		out += renderStrGenResetSeq;
	}
}

void canvas::drawPrimitives(
    const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices,
    const enum drawPrimitives prim
) noexcept
{
    switch (prim) {
        case drawPrimitives::POINTS:
            for (const unsigned int idx : indices)
            {
                const vertex& v = vertices[idx];

                // Map from NDC [-1, 1] to high-res screen space
                const int pixelX = static_cast<int>((v.x + 1.0f) * 0.5f * (width_ - 1));
                const int pixelY = static_cast<int>((1.0f - (v.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));

                if (pixelX < 0 || pixelX >= static_cast<int>(width_)) continue;
                if (pixelY < 0 || pixelY >= static_cast<int>(height_ << 1)) continue;

                cell& c = buffer_[pixelY >> 1][pixelX];

				if (pixelY & 0x1) {
                    if (c.character != L'\u2580') {
                        c.fgColor = c.bgColor;
                    }
                    c.bgColor = v.col;
                }
				else {
                    c.fgColor = v.col;
                }
				
				c.character = L'\u2580';
            }
            break;
		case drawPrimitives::LINES:
            for (size_t i = 0; i < indices.size(); i += 2) {
                if (i + 1 >= indices.size()) break;
                
                const vertex& v1 = vertices[indices[i]];
                const vertex& v2 = vertices[indices[i + 1]];

				const bool colourInterpolate = v1.col != v2.col;
                
                const int x1 = static_cast<int>((v1.x + 1.0f) * 0.5f * (width_ - 1));
                const int y1 = static_cast<int>((1.0f - (v1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
                const int x2 = static_cast<int>((v2.x + 1.0f) * 0.5f * (width_ - 1));
                const int y2 = static_cast<int>((1.0f - (v2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
                
                const int dx = abs(x2 - x1);
                const int dy = abs(y2 - y1);
                const int sx = (x1 < x2) ? 1 : -1;
                const int sy = (y1 < y2) ? 1 : -1;
                int err = dx - dy;
                
                int x = x1, y = y1;
                
                while (true) {
                    // Check bounds
                    if (x >= 0 && x < static_cast<int>(width_) && 
                        y >= 0 && y < static_cast<int>(height_ << 1)) {
                        
                        cell& c = buffer_[y >> 1][x];

                        colour lineColor = v1.col;
						
						if(colourInterpolate) {
							const float t = (dx >= dy && dx > 0)
								? (float)abs(x - x1) / dx
								: (dy > 0 ? (float)abs(y - y1) / dy : 0.0f);

							// Extract RGB components (HSV)
							const float r1 = ((v1.col & colour::red) >> 16) / 255.0f;
							const float g1 = ((v1.col & colour::green) >> 8) / 255.0f;
							const float b1 = (v1.col & colour::blue) / 255.0f;
							const float r2 = ((v2.col & colour::red) >> 16) / 255.0f;
							const float g2 = ((v2.col & colour::green) >> 8) / 255.0f;
							const float b2 = (v2.col & colour::blue) / 255.0f;
							const float r = r1 + t * (r2 - r1);
							const float g = g1 + t * (g2 - g1);
							const float b = b1 + t * (b2 - b1);
							const uint32_t ri = static_cast<uint32_t>(r * 255.0f);
							const uint32_t gi = static_cast<uint32_t>(g * 255.0f);
							const uint32_t bi = static_cast<uint32_t>(b * 255.0f);
							lineColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);
						}
                        
                        if (y & 0x1) {
                            // Bottom half
                            if (c.character != L'\u2580') {
                                c.fgColor = c.bgColor;
                            }
                            c.bgColor = lineColor;
                        } else {
                            // Top half
                            c.fgColor = lineColor;
                        }
                        
                        c.character = L'\u2580';
                    }
                    
                    if (x == x2 && y == y2) break;
                    const int e2 = 2 * err;
                    if (e2 > -dy) {
                        err -= dy;
                        x += sx;
                    }
                    if (e2 < dx) {
                        err += dx;
                        y += sy;
                    }
                }
            }
            break;
        case drawPrimitives::LINE_STRIP:
            for (size_t i = 0; i < indices.size(); i += 1) {
                if (i + 1 >= indices.size()) break;
                
                const vertex& v1 = vertices[indices[i]];
                const vertex& v2 = vertices[indices[i + 1]];

				const bool colourInterpolate = v1.col != v2.col;
                
                const int x1 = static_cast<int>((v1.x + 1.0f) * 0.5f * (width_ - 1));
                const int y1 = static_cast<int>((1.0f - (v1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
                const int x2 = static_cast<int>((v2.x + 1.0f) * 0.5f * (width_ - 1));
                const int y2 = static_cast<int>((1.0f - (v2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
                
                const int dx = abs(x2 - x1);
                const int dy = abs(y2 - y1);
                const int sx = (x1 < x2) ? 1 : -1;
                const int sy = (y1 < y2) ? 1 : -1;
                int err = dx - dy;
                
                int x = x1, y = y1;
                
                while (true) {
                    // Check bounds
                    if (x >= 0 && x < static_cast<int>(width_) && 
                        y >= 0 && y < static_cast<int>(height_ << 1)) {
                        
                        cell& c = buffer_[y >> 1][x];

                        colour lineColor = v1.col;
						
						if(colourInterpolate) {
							const float t = (dx >= dy && dx > 0)
								? (float)abs(x - x1) / dx
								: (dy > 0 ? (float)abs(y - y1) / dy : 0.0f);

							// Extract RGB components (HSV)
							const float r1 = ((v1.col & colour::red) >> 16) / 255.0f;
							const float g1 = ((v1.col & colour::green) >> 8) / 255.0f;
							const float b1 = (v1.col & colour::blue) / 255.0f;
							const float r2 = ((v2.col & colour::red) >> 16) / 255.0f;
							const float g2 = ((v2.col & colour::green) >> 8) / 255.0f;
							const float b2 = (v2.col & colour::blue) / 255.0f;
							const float r = r1 + t * (r2 - r1);
							const float g = g1 + t * (g2 - g1);
							const float b = b1 + t * (b2 - b1);
							const uint32_t ri = static_cast<uint32_t>(r * 255.0f);
							const uint32_t gi = static_cast<uint32_t>(g * 255.0f);
							const uint32_t bi = static_cast<uint32_t>(b * 255.0f);
							lineColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);
						}
                        
                        if (y & 0x1) {
                            // Bottom half
                            if (c.character != L'\u2580') {
                                c.fgColor = c.bgColor;
                            }
                            c.bgColor = lineColor;
                        } else {
                            // Top half
                            c.fgColor = lineColor;
                        }
                        
                        c.character = L'\u2580';
                    }
                    
                    if (x == x2 && y == y2) break;
                    const int e2 = 2 * err;
                    if (e2 > -dy) {
                        err -= dy;
                        x += sx;
                    }
                    if (e2 < dx) {
                        err += dx;
                        y += sy;
                    }
                }
            }
            break;
		case drawPrimitives::TRIANGLES:
			for (size_t i = 0; i < indices.size(); i += 3) {
				if (i + 2 >= indices.size()) break;
				
				const vertex& v1 = vertices[indices[i]];
				const vertex& v2 = vertices[indices[i + 1]];
				const vertex& v3 = vertices[indices[i + 2]];
				
				const float x1f = (v1.x + 1.0f) * 0.5f * (width_ - 1);
				const float y1f = (1.0f - (v1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
				const float x2f = (v2.x + 1.0f) * 0.5f * (width_ - 1);
				const float y2f = (1.0f - (v2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
				const float x3f = (v3.x + 1.0f) * 0.5f * (width_ - 1);
				const float y3f = (1.0f - (v3.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
				
				const int x1 = static_cast<int>(x1f);
				const int y1 = static_cast<int>(y1f);
				const int x2 = static_cast<int>(x2f);
				const int y2 = static_cast<int>(y2f);
				const int x3 = static_cast<int>(x3f);
				const int y3 = static_cast<int>(y3f);
				
				const int minX = std::max(0, std::min({x1, x2, x3}));
				const int maxX = std::min(static_cast<int>(width_) - 1, std::max({x1, x2, x3}));
				const int minY = std::max(0, std::min({y1, y2, y3}));
				const int maxY = std::min(static_cast<int>(height_ << 1) - 1, std::max({y1, y2, y3}));
				
				const float area = 0.5f * ((x2f - x1f) * (y3f - y1f) - (x3f - x1f) * (y2f - y1f));
				if (std::abs(area) < 0.001f) continue;
				
				const float A01 = y1f - y2f;
				const float B01 = x2f - x1f;
				const float C01 = x1f * y2f - x2f * y1f;
				
				const float A12 = y2f - y3f;
				const float B12 = x3f - x2f;
				const float C12 = x2f * y3f - x3f * y2f;
				
				const float A20 = y3f - y1f;
				const float B20 = x1f - x3f;
				const float C20 = x3f * y1f - x1f * y3f;
				
				for (int y = minY; y <= maxY; ++y) {
					for (int x = minX; x <= maxX; ++x) {
						const float px = static_cast<float>(x) + 0.5f;
						const float py = static_cast<float>(y) + 0.5f;
						
						const float w0 = A01 * px + B01 * py + C01;
						const float w1 = A12 * px + B12 * py + C12;
						const float w2 = A20 * px + B20 * py + C20;
						
						const bool inside = (area > 0) ? (w0 >= 0 && w1 >= 0 && w2 >= 0) 
													   : (w0 <= 0 && w1 <= 0 && w2 <= 0);
						
						if (inside) {
							const float invArea = 1.0f / (2.0f * area);
							const float u = w1 * invArea;
							const float v = w2 * invArea;
							const float w = 1.0f - u - v;
							
							const float r1 = ((v1.col & colour::red) >> 16) / 255.0f;
							const float g1 = ((v1.col & colour::green) >> 8) / 255.0f;
							const float b1 = (v1.col & colour::blue) / 255.0f;
							const float r2 = ((v2.col & colour::red) >> 16) / 255.0f;
							const float g2 = ((v2.col & colour::green) >> 8) / 255.0f;
							const float b2 = (v2.col & colour::blue) / 255.0f;
							const float r3 = ((v3.col & colour::red) >> 16) / 255.0f;
							const float g3 = ((v3.col & colour::green) >> 8) / 255.0f;
							const float b3 = (v3.col & colour::blue) / 255.0f;
							
							const float r = u * r1 + v * r2 + w * r3;
							const float g = u * g1 + v * g2 + w * g3;
							const float b = u * b1 + v * b2 + w * b3;
							
							const uint32_t ri = static_cast<uint32_t>(std::clamp(r * 255.0f, 0.0f, 255.0f));
							const uint32_t gi = static_cast<uint32_t>(std::clamp(g * 255.0f, 0.0f, 255.0f));
							const uint32_t bi = static_cast<uint32_t>(std::clamp(b * 255.0f, 0.0f, 255.0f));
							const colour pixelColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);

							cell& c = buffer_[y >> 1][x];
							
							if (y & 0x1) {
								// Bottom half
								if (c.character != L'\u2580') {
									c.fgColor = c.bgColor;
								}
								c.bgColor = pixelColor;
							} else {
								// Top half
								c.fgColor = pixelColor;
							}
							
							c.character = L'\u2580';
						}
					}
				}
			}
			break;
		case drawPrimitives::TRIANGLES_STRIP:
			for (size_t i = 0; i < indices.size(); i += 1) {
				if (i + 2 >= indices.size()) break;
				
				const vertex& v1 = vertices[indices[i]];
				const vertex& v2 = (i % 2 == 0) ? vertices[indices[i + 1]] : vertices[indices[i + 2]];
				const vertex& v3 = (i % 2 == 0) ? vertices[indices[i + 2]] : vertices[indices[i + 1]];

				const int x1 = static_cast<int>((v1.x + 1.0f) * 0.5f * (width_ - 1));
				const int y1 = static_cast<int>((1.0f - (v1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
				const int x2 = static_cast<int>((v2.x + 1.0f) * 0.5f * (width_ - 1));
				const int y2 = static_cast<int>((1.0f - (v2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
				const int x3 = static_cast<int>((v3.x + 1.0f) * 0.5f * (width_ - 1));
				const int y3 = static_cast<int>((1.0f - (v3.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));

				const int minX = std::max(0, std::min({x1, x2, x3}));
				const int maxX = std::min(static_cast<int>(width_) - 1, std::max({x1, x2, x3}));
				const int minY = std::max(0, std::min({y1, y2, y3}));
				const int maxY = std::min(static_cast<int>(height_ << 1) - 1, std::max({y1, y2, y3}));
				
				const float area = 0.5f * abs((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1));
				if (area == 0.0f) continue;
				
				for (int y = minY; y <= maxY; ++y) {
					for (int x = minX; x <= maxX; ++x) {
						const float w1 = ((x2 - x3) * (y - y3) + (y3 - y2) * (x - x3)) / (2.0f * area);
						const float w2 = ((x3 - x1) * (y - y1) + (y1 - y3) * (x - x1)) / (2.0f * area);
						const float w3 = 1.0f - w1 - w2;
						
						if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) {
							const float r1 = ((v1.col & colour::red) >> 16) / 255.0f;
							const float g1 = ((v1.col & colour::green) >> 8) / 255.0f;
							const float b1 = (v1.col & colour::blue) / 255.0f;
							const float r2 = ((v2.col & colour::red) >> 16) / 255.0f;
							const float g2 = ((v2.col & colour::green) >> 8) / 255.0f;
							const float b2 = (v2.col & colour::blue) / 255.0f;
							const float r3 = ((v3.col & colour::red) >> 16) / 255.0f;
							const float g3 = ((v3.col & colour::green) >> 8) / 255.0f;
							const float b3 = (v3.col & colour::blue) / 255.0f;
							
							const float r = w1 * r1 + w2 * r2 + w3 * r3;
							const float g = w1 * g1 + w2 * g2 + w3 * g3;
							const float b = w1 * b1 + w2 * b2 + w3 * b3;
							
							const uint32_t ri = static_cast<uint32_t>(std::clamp(r * 255.0f, 0.0f, 255.0f));
							const uint32_t gi = static_cast<uint32_t>(std::clamp(g * 255.0f, 0.0f, 255.0f));
							const uint32_t bi = static_cast<uint32_t>(std::clamp(b * 255.0f, 0.0f, 255.0f));
							const colour pixelColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);
							
							cell& c = buffer_[y >> 1][x];
							
							if (y & 0x1) {
								// Bottom half
								if (c.character != L'\u2580') {
									c.fgColor = c.bgColor;
								}
								c.bgColor = pixelColor;
							} else {
								// Top half
								c.fgColor = pixelColor;
							}
							
							c.character = L'\u2580';
						}
					}
				}
			}
			break;
    }
}

