#ifndef _WINDOW__H_
#define _WINDOW__H_

#include <glm/glm.hpp>

namespace Core {
	class Window {
	public:
		virtual void Create() = 0;
		virtual bool Present() = 0;
		virtual void SetDimensions(const glm::lowp_u16vec2& dim) = 0;
	};
}

#endif