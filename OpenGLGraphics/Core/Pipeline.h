//
//	Pipeline.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 22/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _PIPELINE__H_
#define _PIPELINE__H_

#include <vector>
#include <memory>
#include "glm.hpp"

namespace Core {
	class Renderable;

	enum class GraphicsAPIS : unsigned char {
		OpenGL,
		Vulkan,
		DirectX11,
		DirectX12
	};

	class Pipeline {
	public:
		virtual void Init() = 0;
		virtual void PreRender() = 0;
		virtual void Render() = 0;
		virtual void PostRender() = 0;
		virtual void Shutdown() = 0;
		virtual void SetDimensions(const glm::lowp_u16vec2& dim) = 0;
		void AddRenderable(const std::weak_ptr<Renderable>& x);
	protected:
		std::vector<std::shared_ptr<Renderable>> mGroupedRenderables;
	};
}

#endif