//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright © 2024. All Rights reserved
//

#include <fstream>
#include "ShaderImporter.h"
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
		*   Imports a Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderProgramImporter::ImportFromFile(const std::string_view& filename) const {
			using Core::Graphics::ShaderProgram;
			using Core::Graphics::Shader;
			using nlohmann::json;

			PageAllocator<TResource<ShaderProgram>> resalloc;
			PageAllocator<ShaderProgram> shadalloc;

			std::ifstream file(filename.data());
			json j = json::parse(file);
			auto& resmg = Singleton<ResourceManager>::Instance();

			ShaderProgram* _shad = shadalloc.allocate();
			AssetReference<Shader> vertex = std::move(resmg.GetResource<Shader>(j["Vertex"].get<std::string>().c_str()));
			AssetReference<Shader> fragment = std::move(resmg.GetResource<Shader>(j["Fragment"].get<std::string>().c_str()));

			shadalloc.construct(_shad, vertex, fragment);
			std::shared_ptr<TResource<ShaderProgram>> const rawResource(resalloc.New(1, std::move(_shad)), [](TResource<ShaderProgram>* p) {
				PageAllocator<TResource<ShaderProgram>> resalloc_;
				PageAllocator<ShaderProgram> shadalloc_;
				auto ptr = p->rawData.release();
				shadalloc_.destroy(ptr);
				shadalloc_.deallocate(ptr);
				resalloc_.destroy(p);
				resalloc_.deallocate(p);
				});

			return std::move(rawResource);
		}

		// ------------------------------------------------------------------------
		/*! Process Shader
		*
		*   Allocates a single Shader (fragment or vertex)
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ProcessShader(const std::string_view& filename, Core::Graphics::Shader::EType type) {
			using Core::Graphics::Shader;

			PageAllocator<TResource<Shader>> resalloc;
			PageAllocator<Shader> shadalloc;

			Shader* const _shad = shadalloc.allocate();
			shadalloc.construct(_shad, filename.data(), type);

			std::shared_ptr<TResource<Shader>> rawResource(resalloc.New(1, std::move(_shad)), [](TResource<Shader>* p) {
				PageAllocator<TResource<Shader>> resalloc_;
				PageAllocator<Shader> shadalloc_;
				auto ptr = p->rawData.release();
				shadalloc_.destroy(ptr);
				shadalloc_.deallocate(ptr);
				resalloc_.destroy(p);
				resalloc_.deallocate(p);
				});

			return std::move(rawResource);
		}

		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports a Vertex Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderImporter<Core::Graphics::Shader::EType::Vertex>::ImportFromFile(const std::string_view& filename) const {
			return std::move(ProcessShader(filename, Core::Graphics::Shader::EType::Vertex));
		}

		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports a Vertex Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderImporter<Core::Graphics::Shader::EType::Fragment>::ImportFromFile(const std::string_view& filename) const {
			return std::move(ProcessShader(filename, Core::Graphics::Shader::EType::Fragment));
		}
	}
}