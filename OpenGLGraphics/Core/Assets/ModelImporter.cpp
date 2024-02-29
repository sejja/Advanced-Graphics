//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright © 2024. All Rights reserved
//

#include <fstream>
#include "ModelImporter.h"
#include "Graphics/Primitives/Model.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/Texture.h"
#include "Dependencies/Json/single_include/json.hpp"
#include "Core/PageAllocator.h"

namespace Core {
	namespace Assets {
		DONTDISCARD std::shared_ptr<IResource> ModelImporter::ImportFromFile(const std::string_view& filename) const {
			using ::Graphics::Primitives::GLBModel;

			PageAllocator<TResource<GLBModel>> resalloc;
			PageAllocator<GLBModel> modealloc;

			std::shared_ptr<TResource<GLBModel>> const rawResource(resalloc.New(1, modealloc.New(1, std::string(filename))), [resalloc, modealloc](TResource<GLBModel>* p) {
				const auto ref = p->Get();
				modealloc.deallocate(ref);
				resalloc.deallocate(p);
				});

			return std::move(rawResource);
		}
	}
}