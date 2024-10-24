#pragma once
#include "WinTerm/events/event.hpp"
	
namespace winTerm
{
	class baseWindow
	{
	public:
		baseWindow() = default;
		virtual ~baseWindow() = default;

		virtual void wndProc(const event& evnt) noexcept = 0;
	};

	class window : public baseWindow
	{
	public:
		window(unsigned int width , unsigned int height) :
			width_(width) , height_(height)
		{}
		~window() = default;

		void wndProc(const event& evnt) noexcept override;

	protected:
		unsigned int width_;
		unsigned int height_;
	};


}

