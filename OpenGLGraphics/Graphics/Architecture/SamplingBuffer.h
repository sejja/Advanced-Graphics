#ifndef _SAMPLINGGBUFFER_
#define _SAMPLINGGBUFFER_

#include <glew.h>
#include "glm.hpp"
#include "Graphics/Tools/FrameBuffer.h"

namespace Core 
{
	namespace Graphics 
	{
		class SamplingBuffer : public FrameBuffer
		{
		public:
			SamplingBuffer();
			~SamplingBuffer();

			void CreateRenderTexture(glm::lowp_u16vec2 dimensions, bool depthonly = false, bool readable = true);
		};
	}
}

#endif // !_SAMPLINGGBUFFER_

