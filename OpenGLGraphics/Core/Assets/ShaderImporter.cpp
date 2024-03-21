//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/02/24
//	Copyright © 2024. All Rights reserved
//

#include <fstream>
#include "ShaderImporter.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Dependencies/Json/single_include/json.hpp"
#include "Core/Memory/PageAllocator.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports a Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderProgramImporter::ImportFromFile(const std::string_view& filename) const {
			using Graphics::ShaderProgram;
			using Graphics::Shader;
			using nlohmann::json;
			using Core::Memory::PageAllocator;

			const PageAllocator<TResource<ShaderProgram>> resalloc;
			const PageAllocator<ShaderProgram> shadalloc;

			std::ifstream file(filename.data());
			json j = json::parse(file);
			ResourceManager& resmg = Singleton<ResourceManager>::Instance();

			ShaderProgram* const _shad = shadalloc.allocate();

			if (j.contains("Geometry")) {
				shadalloc.construct(_shad,
					std::move(resmg.GetResource<Shader>(j["Vertex"].get<std::string>().c_str())),
					std::move(resmg.GetResource<Shader>(j["Fragment"].get<std::string>().c_str())),
					std::move(resmg.GetResource<Shader>(j["Geometry"].get<std::string>().c_str())));
			}
			else {
				shadalloc.construct(_shad,
					std::move(resmg.GetResource<Shader>(j["Vertex"].get<std::string>().c_str())),
					std::move(resmg.GetResource<Shader>(j["Fragment"].get<std::string>().c_str())));
			}

			shadalloc.construct(_shad, 
				std::move(resmg.GetResource<Shader>(j["Vertex"].get<std::string>().c_str())), 
				std::move(resmg.GetResource<Shader>(j["Fragment"].get<std::string>().c_str())));
			Asset<ShaderProgram> rawResource(resalloc.New(1, _shad), [](TResource<ShaderProgram>* const p) {
				const PageAllocator<TResource<ShaderProgram>> resalloc_;
				const PageAllocator<ShaderProgram> shadalloc_;
				shadalloc_.terminate(p->rawData.release());
				resalloc_.terminate(p);
				});

			return std::move(rawResource);
		}

		// ------------------------------------------------------------------------
		/*! Process Shader
		*
		*   Allocates a single Shader (fragment or vertex)
		*/ //----------------------------------------------------------------------
		DONTDISCARD std::shared_ptr<IResource> ProcessShader(const std::string_view& filename, const Graphics::Shader::EType type) {
			using Graphics::Shader;
			using Core::Memory::PageAllocator;

			const PageAllocator<TResource<Shader>> resalloc;
			const PageAllocator<Shader> shadalloc;

			Shader* const _shad = shadalloc.allocate();
			shadalloc.construct(_shad, filename.data(), type);

			std::shared_ptr<TResource<Shader>> rawResource(resalloc.New(1, _shad), [](TResource<Shader>* const p) {
				const PageAllocator<TResource<Shader>> resalloc_;
				const PageAllocator<Shader> shadalloc_;
				shadalloc_.terminate(p->rawData.release());
				resalloc_.terminate(p);
				});

			return std::move(rawResource);
		}

		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports a Vertex Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderImporter<Graphics::Shader::EType::Vertex>::ImportFromFile(const std::string_view& filename) const {
			return std::move(ProcessShader(filename, Graphics::Shader::EType::Vertex));
		}

		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports a Vertex Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderImporter<Graphics::Shader::EType::Fragment>::ImportFromFile(const std::string_view& filename) const {
			return std::move(ProcessShader(filename, Graphics::Shader::EType::Fragment));
		}

		// ------------------------------------------------------------------------
		/*! Import From File
		*
		*   Imports a Vertex Shader from File
		*/ //----------------------------------------------------------------------
		std::shared_ptr<IResource> ShaderImporter<Graphics::Shader::EType::Geometry>::ImportFromFile(const std::string_view& filename) const {
			return std::move(ProcessShader(filename, Graphics::Shader::EType::Geometry));
		}
	}
}