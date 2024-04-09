//
//  SharedResources.cpp
//  OpenGL Graphics
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 . All rights reserved.
//

#include <stack>
#include <algorithm>
#include "ResourceManager.h"
#include "Assets/ModelImporter.h"
#include "Assets/ShaderImporter.h"
#include "Assets/TextureImporter.h"
#include "../Graphics/Primitives/Model.h"

namespace Core {
	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*  Initializes the Resource Manager
		*/ // --------------------------------------------------------------------
		void ResourceManager::Initialize() {
			mImporters.insert({ "obj", std::make_unique<ModelImporter>() });
			mImporters.insert({ "gltf", std::make_unique<ModelImporter>() });
			mImporters.insert({ "png", std::make_unique<TextureImporter>() });
			mImporters.insert({ "jpg",  std::make_unique<TextureImporter>() });
			mImporters.insert({ "jpeg",  std::make_unique<TextureImporter>() });
			mImporters.insert({ "tga",  std::make_unique<TextureImporter>() });
			mImporters.insert({ "shader", std::make_unique<ShaderProgramImporter>() });
			mImporters.insert({ "vert",  std::make_unique<Core::Assets::ShaderImporter<Core::Graphics::Shader::EType::Vertex>>() });
			mImporters.insert({ "frag",  std::make_unique<Core::Assets::ShaderImporter<Core::Graphics::Shader::EType::Fragment>>() });
			mImporters.insert({ "geom",  std::make_unique<Core::Assets::ShaderImporter<Core::Graphics::Shader::EType::Geometry>>() });
		}

		// ------------------------------------------------------------------------
		/*! Get Resource
		*
		*   Gets a resource given the name
		*/ // --------------------------------------------------------------------
		std::shared_ptr<IResource> ResourceManager::GetResource(raw_text name) {
			auto it = mResources.find(name);

			//If we do indeed have the resource
			if (it != mResources.end()) {
				return it->second;
			}

			return AddResource(name);
		}

		// ------------------------------------------------------------------------
		/*! Add Resource
		*
		*   Adds a Resource given the resource, the path and the name
		*/ // --------------------------------------------------------------------
		std::shared_ptr<IResource> ResourceManager::AddResource(raw_text mPath) {
			std::string_view temp_(mPath);

			//assertThis(!temp_.empty(), "Maybe you should think about giving an actual asset path, you fucking clown");
			if (temp_.empty()) return nullptr;

			//Try importing from file
			try {
				IResourceImporter* ireimp_ =
					GetImporterByExtension(temp_.substr(temp_.find_last_of(".") + 1).data());
				mResources.insert(std::make_pair(mPath, std::move(std::shared_ptr<IResource>(ireimp_->ImportFromFile(mPath)))));

				return mResources[mPath];

				//If we could not find an importer
			}
			catch (...) {
				return nullptr;
			}
		}

		// ------------------------------------------------------------------------
		/*! Get Importer By Extension
		*
		*   Gets an importer by it's extension
		*/ // --------------------------------------------------------------------
		IResourceImporter* ResourceManager::GetImporterByExtension(raw_text ext) const {
			return mImporters.at(ext).get();
		}

		// ------------------------------------------------------------------------
		/*! Remove Resource
		*
		*   Removes a resource
		*/ // --------------------------------------------------------------------
		void ResourceManager::RemoveResource(raw_text name) {
			mResources.erase(name);
		}

		void ResourceManager::ShutDown() {
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // --------------------------------------------------------------------
		IResource::~IResource() {}
	}
}