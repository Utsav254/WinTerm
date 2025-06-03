#pragma once
#include <stdexcept>
#include <format>
#include <variant>
#include <vector>

#include "rect.hpp"
#include "defs.hpp"
#include "matmath.hpp"

namespace winTerm
{
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

	using vsOut = vec4;
	using fsOut = vec4;

	template<typename customOutputType, typename vertexIn, typename... uniforms>
	using vertexShader = void(vsOut&, customOutputType&, const vertexIn&, const uniforms ...);

	template<typename customInputType, typename... uniforms>
	using fragmentShader = void(fsOut&, const customInputType&, const uniforms ...);

	struct canvas {
	public:
		canvas(
			const unsigned int width, const unsigned int height,
			const unsigned int x = 0, const unsigned int y = 0,
			wchar_t character = L' ', colour fg = colour::white, colour bg = colour::black,
			emphasis emph = emphasis::norm,
			const canvMsg msg = canvMsg::OPTIMISE):

			width_(width) , height_(height),
			x_(x), y_(y),
			buffer_chars_(height , std::vector<wchar_t>(width , character)),
			buffer_fg_(height , std::vector<colour>(width , fg)),
			buffer_bg_(height , std::vector<colour>(width , bg)),
			buffer_emph_(height , std::vector<emphasis>(width , emph)),
			renderStrGenResetSeq(std::format("\x1b[1B\x1b[{:d}D", width_)),
			message_(msg)
		{
			if((width < 1 || height < 1) && msg != canvMsg::END)
				throw std::out_of_range(std::format("invalid size({:d} , {:d})" , width , height));
		}

		~canvas() = default;

		canvas(const canvas& ) = delete;
		canvas& operator=(const canvas&) = delete;

		wchar_t& at(const unsigned int row , const unsigned int column);

		void clear() noexcept;
		
		void setBackground(const colour col) noexcept;
		void setForeground(const colour col) noexcept;

		void addText(const std::string& str , unsigned int row , unsigned int column , 
			   const colour fg , const colour bg , emphasis emph);

		void drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept;
		
		void setBorder(const borderStyle bs, const colour fg) noexcept;

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
			auto write_pixel = [&](int x, int y, const vec4& fragCol) -> void {
				if (x < 0 || x >= static_cast<int>(width_) || 
					y < 0 || y >= static_cast<int>(height_ << 1)) return;
					
				wchar_t& character = buffer_chars_[y >> 1][x];
				colour& fgColor = buffer_fg_[y >> 1][x];
				colour& bgColor = buffer_bg_[y >> 1][x];
				
				// SIMD color conversion
				vec4 scaledCol = fragCol;
				scaledCol.mul(255.0f);
				
				const uint32_t ri = static_cast<uint32_t>(scaledCol.c.r);
				const uint32_t gi = static_cast<uint32_t>(scaledCol.c.g);
				const uint32_t bi = static_cast<uint32_t>(scaledCol.c.b);
				const colour finalCol = static_cast<colour>((ri << 16) | (gi << 8) | bi);

				if (y & 0x1) {
					if (character != L'\u2580') {
						fgColor = bgColor;
					}
					bgColor = finalCol;
				} else {
					fgColor = finalCol;
				}
				
				character = L'\u2580';
			};

			// Optimized line drawing with SIMD interpolation
			auto draw_line = [&](const vec4& clipPos1, const vec4& clipPos2, 
								 const vec4& col1, const vec4& col2) -> void {
				const int x1 = static_cast<int>((clipPos1.p.x + 1.0f) * 0.5f * (width_ - 1));
				const int y1 = static_cast<int>((1.0f - (clipPos1.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
				const int x2 = static_cast<int>((clipPos2.p.x + 1.0f) * 0.5f * (width_ - 1));
				const int y2 = static_cast<int>((1.0f - (clipPos2.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));
				
				const int dx = abs(x2 - x1);
				const int dy = abs(y2 - y1);
				const int sx = (x1 < x2) ? 1 : -1;
				const int sy = (y1 < y2) ? 1 : -1;
				int err = dx - dy;
				
				const float inv_length = (dx >= dy && dx > 0) ? 1.0f / dx : (dy > 0 ? 1.0f / dy : 0.0f);
				
				int x = x1, y = y1;
				
				while (true) {
					const float t = (dx >= dy && dx > 0)
						? static_cast<float>(std::abs(x - x1)) * inv_length
						: (dy > 0 ? static_cast<float>(std::abs(y - y1)) * inv_length : 0.0f);

					vec4 fragCol;
					lerp(col1, col2, fragCol, t);
					write_pixel(x, y, fragCol);
					
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
			};

			switch (prim) {
				case drawPrimitives::POINTS:
					for (const unsigned int idx : indices) {
						const vsIn& v = vertices[idx];

						vec4 clipPos;
						vsOutfsIn vofi;
						vs(clipPos, vofi, v, unifs ...);

						const int pixelX = static_cast<int>((clipPos.p.x + 1.0f) * 0.5f * (width_ - 1));
						const int pixelY = static_cast<int>((1.0f - (clipPos.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1));

						vec4 fragCol;
						fs(fragCol, vofi, unifs...);
						write_pixel(pixelX, pixelY, fragCol);
					}
					break;

				case drawPrimitives::LINES:
					for (size_t i = 0; i < indices.size(); i += 2) {
						if (i + 1 >= indices.size()) break;
						
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = vertices[indices[i + 1]];
						
						vec4 clipPos1, clipPos2;
						vsOutfsIn vofi1, vofi2;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						
						vec4 col1, col2;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						
						draw_line(clipPos1, clipPos2, col1, col2);
					}
					break;

				case drawPrimitives::LINE_STRIP:
					for (size_t i = 0; i < indices.size() - 1; i++) {
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = vertices[indices[i + 1]];

						vec4 clipPos1, clipPos2;
						vsOutfsIn vofi1, vofi2;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						
						vec4 col1, col2;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						
						draw_line(clipPos1, clipPos2, col1, col2);
					}
					break;

				case drawPrimitives::TRIANGLES:
					for (size_t i = 0; i < indices.size(); i += 3) {
						if (i + 2 >= indices.size()) break;
						
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = vertices[indices[i + 1]];
						const vsIn& v3 = vertices[indices[i + 2]];
						
						vec4 clipPos1, clipPos2, clipPos3;
						vsOutfsIn vofi1, vofi2, vofi3;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						vs(clipPos3, vofi3, v3, unifs ...);
						
						vec4 col1, col2, col3;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						fs(col3, vofi3, unifs ...);
						
						const float x1f = (clipPos1.p.x + 1.0f) * 0.5f * (width_ - 1);
						const float y1f = (1.0f - (clipPos1.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						const float x2f = (clipPos2.p.x + 1.0f) * 0.5f * (width_ - 1);
						const float y2f = (1.0f - (clipPos2.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						const float x3f = (clipPos3.p.x + 1.0f) * 0.5f * (width_ - 1);
						const float y3f = (1.0f - (clipPos3.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						
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
						
						// Pre-compute reciprocal and edge function constants
						const float invArea = 1.0f / (2.0f * area);
						const float A01 = y1f - y2f;
						const float B01 = x2f - x1f;
						const float C01 = x1f * y2f - x2f * y1f;
						
						const float A12 = y2f - y3f;
						const float B12 = x3f - x2f;
						const float C12 = x2f * y3f - x3f * y2f;
						
						const float A20 = y3f - y1f;
						const float B20 = x1f - x3f;
						const float C20 = x3f * y1f - x1f * y3f;
						
						// Optimized rasterization with incremental edge functions
						for (int y = minY; y <= maxY; ++y) {
							const float py = static_cast<float>(y) + 0.5f;
							
							// Pre-calculate py-dependent terms
							const float py_B01 = B01 * py;
							const float py_B12 = B12 * py;
							const float py_B20 = B20 * py;
							const float base_w0 = py_B01 + C01;
							const float base_w1 = py_B12 + C12;
							const float base_w2 = py_B20 + C20;
							
							// Start of scanline edge function values
							float w0 = A01 * (static_cast<float>(minX) + 0.5f) + base_w0;
							float w1 = A12 * (static_cast<float>(minX) + 0.5f) + base_w1;
							float w2 = A20 * (static_cast<float>(minX) + 0.5f) + base_w2;
							
							for (int x = minX; x <= maxX; ++x) {
								const bool inside = (area > 0) ? (w0 >= 0 && w1 >= 0 && w2 >= 0) 
															   : (w0 <= 0 && w1 <= 0 && w2 <= 0);
								
								if (inside) {
									const float u = w1 * invArea;
									const float v = w2 * invArea;
									const float w = 1.0f - u - v;
									const vec4 weights(u, v, w);
									
									vec4 fragCol;
									weighted_sum(col1, col2, col3, weights, fragCol);
									write_pixel(x, y, fragCol);
								}
								
								// Increment edge functions
								w0 += A01;
								w1 += A12;
								w2 += A20;
							}
						}
					}
					break;

				case drawPrimitives::TRIANGLE_STRIP:
					for (size_t i = 0; i < indices.size() - 2; i++) {
						const vsIn& v1 = vertices[indices[i]];
						const vsIn& v2 = (i % 2 == 0) ? vertices[indices[i + 1]] : vertices[indices[i + 2]];
						const vsIn& v3 = (i % 2 == 0) ? vertices[indices[i + 2]] : vertices[indices[i + 1]];

						vec4 clipPos1, clipPos2, clipPos3;
						vsOutfsIn vofi1, vofi2, vofi3;
						vs(clipPos1, vofi1, v1, unifs ...);
						vs(clipPos2, vofi2, v2, unifs ...);
						vs(clipPos3, vofi3, v3, unifs ...);
						
						vec4 col1, col2, col3;
						fs(col1, vofi1, unifs ...);
						fs(col2, vofi2, unifs ...);
						fs(col3, vofi3, unifs ...);

						const float x1f = (clipPos1.p.x + 1.0f) * 0.5f * (width_ - 1);
						const float y1f = (1.0f - (clipPos1.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						const float x2f = (clipPos2.p.x + 1.0f) * 0.5f * (width_ - 1);
						const float y2f = (1.0f - (clipPos2.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);
						const float x3f = (clipPos3.p.x + 1.0f) * 0.5f * (width_ - 1);
						const float y3f = (1.0f - (clipPos3.p.y + 1.0f) * 0.5f) * ((height_ << 1) - 1);

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
						
						// Same optimized rasterization as TRIANGLES
						const float invArea = 1.0f / (2.0f * area);
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
							const float py = static_cast<float>(y) + 0.5f;
							
							const float py_B01 = B01 * py;
							const float py_B12 = B12 * py;
							const float py_B20 = B20 * py;
							const float base_w0 = py_B01 + C01;
							const float base_w1 = py_B12 + C12;
							const float base_w2 = py_B20 + C20;
							
							float w0 = A01 * (static_cast<float>(minX) + 0.5f) + base_w0;
							float w1 = A12 * (static_cast<float>(minX) + 0.5f) + base_w1;
							float w2 = A20 * (static_cast<float>(minX) + 0.5f) + base_w2;
							
							for (int x = minX; x <= maxX; ++x) {
								const bool inside = (area > 0) ? (w0 >= 0 && w1 >= 0 && w2 >= 0) 
															   : (w0 <= 0 && w1 <= 0 && w2 <= 0);
								
								if (inside) {
									const float u = w1 * invArea;
									const float v = w2 * invArea;
									const float w = 1.0f - u - v;
									const vec4 weights(u, v, w);
									
									vec4 fragCol;
									weighted_sum(col1, col2, col3, weights, fragCol);
									write_pixel(x, y, fragCol);
								}
								
								w0 += A01;
								w1 += A12;
								w2 += A20;
							}
						}
					}
					break;
			}
		}
	private:
		unsigned int width_ , height_;
		unsigned int x_, y_;

		std::vector<std::vector<wchar_t>> buffer_chars_;
		std::vector<std::vector<colour>> buffer_fg_;
		std::vector<std::vector<colour>> buffer_bg_;
		std::vector<std::vector<emphasis>> buffer_emph_;

		const std::string renderStrGenResetSeq;

		canvMsg message_;
	};
	
}
