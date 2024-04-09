//
//  SharedResources.cpp
//  OpenGL Graphics
//
//  Created by Diego Revilla on 9/4/24.
//  Copyright � 2024 . All rights reserved.
//

#include <stack>
#include <algorithm>
#include <cassert>
#include "ResourceManager.h"
#include "ModelImporter.h"
#include "ShaderImporter.h"
#include "TextureImporter.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*  Initializes the Resource Manager
		*/ // --------------------------------------------------------------------
		void ResourceManager::Initialize() {
			const auto modelImporter = std::make_shared<ModelImporter>();
			const auto textureImporter = std::make_shared<TextureImporter>();
			mImporters.insert({ "obj", modelImporter });
			mImporters.insert({ "gltf", modelImporter });
			mImporters.insert({ "png", textureImporter });
			mImporters.insert({ "jpg",  textureImporter });
			mImporters.insert({ "jpeg",  textureImporter });
			mImporters.insert({ "tga",  textureImporter });
			mImporters.insert({ "shader", std::make_shared<ShaderProgramImporter>() });
			mImporters.insert({ "vert",  std::make_shared<Core::Assets::ShaderImporter<Core::Graphics::Shader::EType::Vertex>>() });
			mImporters.insert({ "frag",  std::make_shared<Core::Assets::ShaderImporter<Core::Graphics::Shader::EType::Fragment>>() });
			mImporters.insert({ "geom",  std::make_shared<Core::Assets::ShaderImporter<Core::Graphics::Shader::EType::Geometry>>() });
		}

		// ------------------------------------------------------------------------
		/*! Get Resource
		*
		*   Gets a resource given the name
		*/ // --------------------------------------------------------------------
		std::shared_ptr<IResource> ResourceManager::GetResource(raw_text name) {
			assert(strlen(name) > 0, "Maybe you should think about giving an actual asset path, you clown");
			std::unordered_map<std::string, std::shared_ptr<IResource>>::const_iterator it = mResources.find(name);

			//If we do indeed have the resource
			return it != mResources.end() ? it->second : AddResource(name);
		}

		// ------------------------------------------------------------------------
		/*! Add Resource
		*
		*   Adds a Resource given the resource, the path and the name
		*/ // --------------------------------------------------------------------
		std::shared_ptr<IResource> ResourceManager::AddResource(raw_text mPath) {
			std::string_view temp_(mPath);

			assert(!temp_.empty(), "Maybe you should think about giving an actual asset path, you clown");

			//Try importing from file
			try {
				return mResources.insert(std::make_pair(mPath,
					std::move(
						std::shared_ptr<IResource>(GetImporterByExtension(temp_.substr(temp_.find_last_of(".") + 1).data())->
						ImportFromFile(mPath))))).first->second;

				//If we could not find an importer
			} catch (std::exception & e) {
				throw ResourceManagerException("Could not find an importer for the file");
			}
		}

		// ------------------------------------------------------------------------
		/*! Get Importer By Extension
		*
		*   Gets an importer by it's extension
		*/ // --------------------------------------------------------------------
		std::shared_ptr<IResourceImporter> ResourceManager::GetImporterByExtension(raw_text ext) const {
			return mImporters.at(ext);
		}

		// ------------------------------------------------------------------------
		/*! Remove Resource
		*
		*   Removes a resource
		*/ // ---------------------------------------------------------------------
		void ResourceManager::RemoveResource(raw_text name) {
			mResources.erase(name);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // --------------------------------------------------------------------
		IResource::~IResource() {}
	}
}