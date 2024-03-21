//
//	ShaderImporter.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 22/02/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _SHADER_IMPORTER__H_
#define _SHADER_IMPORTER__H_

#include "Core/ResourceManager.h"

namespace Core {
	namespace Assets {
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

		template<>
		class ShaderImporter<Core::Graphics::Shader::EType::Geometry> : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};
	}
}

#endif