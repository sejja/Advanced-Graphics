//
//	Importers.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _IMPORTERS__H_
#define _IMPORTERS__H_

#include "../ResourceManager.h"
#include "Core/PageAllocator.h"

namespace Core {
	namespace Assets {
		class ModelImporter : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};

		class TextureImporter : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};

		class ShaderProgramImporter : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};

		template<Core::Graphics::Shader::EType TYPE>
		class ShaderImporter : public IResourceImporter {};

		template<>
		class ShaderImporter<Core::Graphics::Shader::EType::Vertex> : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};

		template<>
		class ShaderImporter<Core::Graphics::Shader::EType::Fragment> : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};
	}
}

#endif