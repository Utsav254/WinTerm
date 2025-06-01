#pragma once
#include <stdexcept>
#include <format>
#include <variant>
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

	enum class drawPrimitives
	{
		POINTS,
		LINES,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
	};

	struct vsOut
	{
		float x, y, z, w;
	};

	template<typename customOutputType, typename vertexIn, typename... uniforms>
	using vertexShader = void(vsOut&, customOutputType&, const vertexIn&, const uniforms ...);

	template<typename customInputType, typename... uniforms>
	using fragmentShader = void(colour&, const customInputType&, const uniforms ...);

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

		template<
			typename vsIn,
			typename vsOutfsIn = std::monostate,
			typename ... uniforms
		>
		void draw(
				const std::vector<vsIn>& vertices,
				const std::vector<unsigned int>& indices,
				const drawPrimitives prim,
				vertexShader<vsOutfsIn, vsIn, uniforms ...> vs,
				fragmentShader<vsOutfsIn, uniforms ...> fs,
				const uniforms ... unifs
		) noexcept
		{
			switch (prim) {
				case drawPrimitives::POINTS:
					for (const unsigned int idx : indices)
					{
						const vsIn& v = vertices[idx];

						vsOut clipPos;
						vsOutfsIn vofi;
						vs(clipPos, vofi, v, unifs ...);

						const int pixelX = static_cast<int>((clipPos.x + 1.0f) * 0.5f * (width_ - 1));
						const int pixelY = static_cast<int>((1.0f - (clipPos.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));

						if (pixelX < 0 || pixelX >= static_cast<int>(width_)) continue;
						if (pixelY < 0 || pixelY >= static_cast<int>(height_ << 1)) continue;

						cell& c = buffer_[pixelY >> 1][pixelX];

						colour fragCol;
						fs(fragCol, vofi, unifs...);

						if (pixelY & 0x1) {
							if (c.character != L'\u2580') {
								c.fgColor = c.bgColor;
							}
							c.bgColor = fragCol;
						}
						else {
							c.fgColor = fragCol;
						}
						
						c.character = L'\u2580';
					}
					break;

				case drawPrimitives::LINES:
					for (size_t i = 0; i < indices.size(); i += 2) {
						if (i + 1 >= indices.size()) break;
						
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = vertices[indices[i + 1]];
						
						vsOut clipPos1, clipPos2;
						vsOutfsIn vofi1, vofi2;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						
						// Get colors from fragment shader
						colour col1, col2;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						
						const bool colourInterpolate = col1 != col2;
						
						const int x1 = static_cast<int>((clipPos1.x + 1.0f) * 0.5f * (width_ - 1));
						const int y1 = static_cast<int>((1.0f - (clipPos1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
						const int x2 = static_cast<int>((clipPos2.x + 1.0f) * 0.5f * (width_ - 1));
						const int y2 = static_cast<int>((1.0f - (clipPos2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
						
						const int dx = abs(x2 - x1);
						const int dy = abs(y2 - y1);
						const int sx = (x1 < x2) ? 1 : -1;
						const int sy = (y1 < y2) ? 1 : -1;
						int err = dx - dy;
						
						int x = x1, y = y1;
						
						while (true) {
							if (x >= 0 && x < static_cast<int>(width_) && 
								y >= 0 && y < static_cast<int>(height_ << 1)) {
								
								cell& c = buffer_[y >> 1][x];
								colour lineColor = col1;
								
								if(colourInterpolate) {
									const float t = (dx >= dy && dx > 0)
										? (float)abs(x - x1) / dx
										: (dy > 0 ? (float)abs(y - y1) / dy : 0.0f);

									const float r1 = ((col1 & colour::red) >> 16) / 255.0f;
									const float g1 = ((col1 & colour::green) >> 8) / 255.0f;
									const float b1 = (col1 & colour::blue) / 255.0f;
									const float r2 = ((col2 & colour::red) >> 16) / 255.0f;
									const float g2 = ((col2 & colour::green) >> 8) / 255.0f;
									const float b2 = (col2 & colour::blue) / 255.0f;
									const float r = r1 + t * (r2 - r1);
									const float g = g1 + t * (g2 - g1);
									const float b = b1 + t * (b2 - b1);
									const uint32_t ri = static_cast<uint32_t>(r * 255.0f);
									const uint32_t gi = static_cast<uint32_t>(g * 255.0f);
									const uint32_t bi = static_cast<uint32_t>(b * 255.0f);
									lineColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);
								}
								
								if (y & 0x1) {
									if (c.character != L'\u2580') {
										c.fgColor = c.bgColor;
									}
									c.bgColor = lineColor;
								} else {
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
					for (size_t i = 0; i < indices.size() - 1; i++) {
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = vertices[indices[i + 1]];
						
						vsOut clipPos1, clipPos2;
						vsOutfsIn vofi1, vofi2;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						
						colour col1, col2;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						
						const bool colourInterpolate = col1 != col2;
						
						const int x1 = static_cast<int>((clipPos1.x + 1.0f) * 0.5f * (width_ - 1));
						const int y1 = static_cast<int>((1.0f - (clipPos1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
						const int x2 = static_cast<int>((clipPos2.x + 1.0f) * 0.5f * (width_ - 1));
						const int y2 = static_cast<int>((1.0f - (clipPos2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
						
						const int dx = abs(x2 - x1);
						const int dy = abs(y2 - y1);
						const int sx = (x1 < x2) ? 1 : -1;
						const int sy = (y1 < y2) ? 1 : -1;
						int err = dx - dy;
						
						int x = x1, y = y1;
						
						while (true) {
							if (x >= 0 && x < static_cast<int>(width_) && 
								y >= 0 && y < static_cast<int>(height_ << 1)) {
								
								cell& c = buffer_[y >> 1][x];
								colour lineColor = col1;
								
								if(colourInterpolate) {
									const float t = (dx >= dy && dx > 0)
										? (float)abs(x - x1) / dx
										: (dy > 0 ? (float)abs(y - y1) / dy : 0.0f);

									const float r1 = ((col1 & colour::red) >> 16) / 255.0f;
									const float g1 = ((col1 & colour::green) >> 8) / 255.0f;
									const float b1 = (col1 & colour::blue) / 255.0f;
									const float r2 = ((col2 & colour::red) >> 16) / 255.0f;
									const float g2 = ((col2 & colour::green) >> 8) / 255.0f;
									const float b2 = (col2 & colour::blue) / 255.0f;
									const float r = r1 + t * (r2 - r1);
									const float g = g1 + t * (g2 - g1);
									const float b = b1 + t * (b2 - b1);
									const uint32_t ri = static_cast<uint32_t>(r * 255.0f);
									const uint32_t gi = static_cast<uint32_t>(g * 255.0f);
									const uint32_t bi = static_cast<uint32_t>(b * 255.0f);
									lineColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);
								}
								
								if (y & 0x1) {
									if (c.character != L'\u2580') {
										c.fgColor = c.bgColor;
									}
									c.bgColor = lineColor;
								} else {
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
						
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = vertices[indices[i + 1]];
						const vsIn& v3 = vertices[indices[i + 2]];
						
						vsOut clipPos1, clipPos2, clipPos3;
						vsOutfsIn vofi1, vofi2, vofi3;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						vs(clipPos3, vofi3, v3, unifs ...);
						
						colour col1, col2, col3;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						fs(col3, vofi3, unifs ...);
						
						const float x1f = (clipPos1.x + 1.0f) * 0.5f * (width_ - 1);
						const float y1f = (1.0f - (clipPos1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						const float x2f = (clipPos2.x + 1.0f) * 0.5f * (width_ - 1);
						const float y2f = (1.0f - (clipPos2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						const float x3f = (clipPos3.x + 1.0f) * 0.5f * (width_ - 1);
						const float y3f = (1.0f - (clipPos3.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						
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
									
									const float r1 = ((col1 & colour::red) >> 16) / 255.0f;
									const float g1 = ((col1 & colour::green) >> 8) / 255.0f;
									const float b1 = (col1 & colour::blue) / 255.0f;
									const float r2 = ((col2 & colour::red) >> 16) / 255.0f;
									const float g2 = ((col2 & colour::green) >> 8) / 255.0f;
									const float b2 = (col2 & colour::blue) / 255.0f;
									const float r3 = ((col3 & colour::red) >> 16) / 255.0f;
									const float g3 = ((col3 & colour::green) >> 8) / 255.0f;
									const float b3 = (col3 & colour::blue) / 255.0f;
									
									const float r = u * r1 + v * r2 + w * r3;
									const float g = u * g1 + v * g2 + w * g3;
									const float b = u * b1 + v * b2 + w * b3;
									
									const uint32_t ri = static_cast<uint32_t>(std::clamp(r * 255.0f, 0.0f, 255.0f));
									const uint32_t gi = static_cast<uint32_t>(std::clamp(g * 255.0f, 0.0f, 255.0f));
									const uint32_t bi = static_cast<uint32_t>(std::clamp(b * 255.0f, 0.0f, 255.0f));
									const colour pixelColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);

									cell& c = buffer_[y >> 1][x];
									
									if (y & 0x1) {
										if (c.character != L'\u2580') {
											c.fgColor = c.bgColor;
										}
										c.bgColor = pixelColor;
									} else {
										c.fgColor = pixelColor;
									}
									
									c.character = L'\u2580';
								}
							}
						}
					}
					break;
					
				case drawPrimitives::TRIANGLE_STRIP:
					for (size_t i = 0; i < indices.size() - 2; i++) {
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = (i % 2 == 0) ? vertices[indices[i + 1]] : vertices[indices[i + 2]];
						const vsIn& v3 = (i % 2 == 0) ? vertices[indices[i + 2]] : vertices[indices[i + 1]];

						vsOut clipPos1, clipPos2, clipPos3;
						vsOutfsIn vofi1, vofi2, vofi3;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						vs(clipPos3, vofi3, v3, unifs ...);
						
						colour col1, col2, col3;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						fs(col3, vofi3, unifs ...);

						const int x1 = static_cast<int>((clipPos1.x + 1.0f) * 0.5f * (width_ - 1));
						const int y1 = static_cast<int>((1.0f - (clipPos1.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
						const int x2 = static_cast<int>((clipPos2.x + 1.0f) * 0.5f * (width_ - 1));
						const int y2 = static_cast<int>((1.0f - (clipPos2.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
						const int x3 = static_cast<int>((clipPos3.x + 1.0f) * 0.5f * (width_ - 1));
						const int y3 = static_cast<int>((1.0f - (clipPos3.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));

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
									const float r1 = ((col1 & colour::red) >> 16) / 255.0f;
									const float g1 = ((col1 & colour::green) >> 8) / 255.0f;
									const float b1 = (col1 & colour::blue) / 255.0f;
									const float r2 = ((col2 & colour::red) >> 16) / 255.0f;
									const float g2 = ((col2 & colour::green) >> 8) / 255.0f;
									const float b2 = (col2 & colour::blue) / 255.0f;
									const float r3 = ((col3 & colour::red) >> 16) / 255.0f;
									const float g3 = ((col3 & colour::green) >> 8) / 255.0f;
									const float b3 = (col3 & colour::blue) / 255.0f;
									
									const float r = w1 * r1 + w2 * r2 + w3 * r3;
									const float g = w1 * g1 + w2 * g2 + w3 * g3;
									const float b = w1 * b1 + w2 * b2 + w3 * b3;
									
									const uint32_t ri = static_cast<uint32_t>(std::clamp(r * 255.0f, 0.0f, 255.0f));
									const uint32_t gi = static_cast<uint32_t>(std::clamp(g * 255.0f, 0.0f, 255.0f));
									const uint32_t bi = static_cast<uint32_t>(std::clamp(b * 255.0f, 0.0f, 255.0f));
									const colour pixelColor = static_cast<colour>((ri << 16) | (gi << 8) | bi);
									
									cell& c = buffer_[y >> 1][x];
									
									if (y & 0x1) {
										if (c.character != L'\u2580') {
											c.fgColor = c.bgColor;
										}
										c.bgColor = pixelColor;
									} else {
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

	private:
		unsigned int width_ , height_;
		unsigned int x_, y_;
		std::vector<std::vector<cell>> buffer_;
		colour background_;

		const std::string renderStrGenResetSeq;

		canvMsg message_;
	};
	
}
