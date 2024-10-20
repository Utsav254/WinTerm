#pragma once

#include "event.hpp"
#include <cstddef>

namespace winTerm
{
	constexpr std::size_t eventQueueSize = 64;
	
	// push or pop events
	// will not throw even if pushing was unsuccessful
	void pushEvent(const event& evnt) noexcept;
	void popEvent(event& evnt) noexcept;
}
