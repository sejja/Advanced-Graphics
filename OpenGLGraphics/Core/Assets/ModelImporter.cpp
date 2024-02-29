//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright © 2024. All Rights reserved
//

#include "ModelImporter.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Core/PageAllocator.h"
#include "Core/ResourceManager.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*  Imports a model from a file
		*/ // ---------------------------------------------------------------------
		std::shared_ptr<IResource> ModelImporter::ImportFromFile(const std::string_view& filename) const {
			using ::Graphics::Primitives::GLBModel;

			const PageAllocator<TResource<GLBModel>> resalloc;
			const PageAllocator<GLBModel> modealloc;

			Asset<GLBModel> rawResource(resalloc.New(1, 
				modealloc.New(1, std::string(filename))), [resalloc, modealloc](TResource<GLBModel>* const p) {
					const PageAllocator<TResource<GLBModel>> resalloc;
					const PageAllocator<GLBModel> modealloc;
					modealloc.terminate(p->Get());
					resalloc.deallocate(p);
				});

			return rawResource;
		}
	}
}