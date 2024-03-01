//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright © 2024. All Rights reserved
//

#include "TextureImporter.h"
#include "Graphics/Primitives/Texture.h"
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
			const PageAllocator<TResource<Texture>> resalloc;
			const PageAllocator<Texture> texalloc;

			Asset<Texture> rawResource(resalloc.New(1, texalloc.New()), [](TResource<Texture>* const p) {
				const PageAllocator<TResource<Texture>> resalloc;
				const PageAllocator<Texture> texalloc;
				texalloc.terminate(p->rawData.release());
				resalloc.deallocate(p);
				});

			rawResource->rawData->LoadFromFile(filename.data());

			return std::move(rawResource);
		}
	}
}