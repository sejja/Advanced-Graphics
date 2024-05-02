//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/24
//	Copyright © 2024. All Rights reserved
//

#include "TextureImporter.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/Memory/PageAllocator.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports Textures from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> Assets::TextureImporter::ImportFromFile(const std::string_view& filename) const {
			using Core::Graphics::Texture;
			using Core::Memory::PageAllocator;
			const PageAllocator<TResource<Texture>> resalloc;
			const PageAllocator<Texture> texalloc;

			Asset<Texture> rawResource(resalloc.New(1, texalloc.New()), [resalloc, texalloc](TResource<Texture>* const p) {
				const PageAllocator<TResource<Texture>> resalloc;
				const PageAllocator<Texture> texalloc;
				texalloc.terminate(p->Get());
				resalloc.terminate(p);
				});

			rawResource->Get()->LoadFromFile(filename.data());

			return rawResource;
		}
	}
}