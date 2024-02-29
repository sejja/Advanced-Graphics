//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright © 2024. All Rights reserved
//

#include <fstream>
#include "TextureImporter.h"
#include "Graphics/Primitives/Model.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/Texture.h"
#include "Dependencies/Json/single_include/json.hpp"
#include "Core/PageAllocator.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports Textures from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> Assets::TextureImporter::ImportFromFile(const std::string_view& filename) const {
			using Core::Graphics::Texture;
			PageAllocator<TResource<Texture>> resalloc;
			PageAllocator<Texture> texalloc;

			std::shared_ptr<TResource<Texture>> rawResource(resalloc.New(1, texalloc.New()), [](TResource<Texture>* p) {
				PageAllocator<TResource<Texture>> resalloc;
				PageAllocator<Texture> texalloc;
				auto ptr = p->rawData.release();
				texalloc.destroy(ptr);
				texalloc.deallocate(ptr);
				resalloc.destroy(p);
				resalloc.deallocate(p);
				});

			rawResource->rawData->LoadFromFile(filename.data());

			return std::move(rawResource);
		}
	}
}