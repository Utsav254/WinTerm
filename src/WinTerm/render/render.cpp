#include "WinTerm/render/render.hpp"
#include "generics/boundedQueue.hpp"

namespace winTerm
{
	// queue of device contexts :
	boundedQueue<std::unique_ptr<canvas> , numCanvas> canvasPool;
}
