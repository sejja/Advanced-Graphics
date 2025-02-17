//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright � 2024. All Rights reserved
//

#include "ModelImporter.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Core/Memory/PageAllocator.h"
#include "Core/Assets/ResourceManager.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*  Imports a model from a file
		*/ // ---------------------------------------------------------------------
		std::shared_ptr<IResource> ModelImporter::ImportFromFile(const std::string_view& filename) const {
			using ::Graphics::Primitives::Model;
			using ::Core::Memory::PageAllocator;

			static PageAllocator<TResource<Model>> resalloc;
			static PageAllocator<Model> modealloc;

			Asset<Model> rawResource(resalloc.New(1, 
				modealloc.New(1, std::string(filename))), [](TResource<Model>* const p) {
					modealloc.terminate(p->Get());
					resalloc.deallocate(p);
				});

			return rawResource;
		}
	}
}