//
//  SharedResources.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 . All rights reserved.
//

#ifndef _RESOURCE_MANAGER__H_
#define _RESOURCE_MANAGER__H_

#include "../CommonDefines.h"
#include <string>
#include <memory>
#include <unordered_map>
#include "Memory/Allocator.h"
#include "Singleton.h"
#include "Graphics/Primitives/Shader.h"
#include <iostream>

struct IResource {
	virtual ~IResource();
};

class IResourceImporter {
public:
	virtual std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const = 0;
};

template<typename Ty_>
class TResource : public IResource {
public:
	TResource(Ty_* data) :
		rawData(data){
		
	}
	~TResource() {}
	
	// ------------------------------------------------------------------------
	/*! Get
	*
	*   Gets the Resource as something
	*/ // --------------------------------------------------------------------
	DONTDISCARD Ty_ inline* Get() {
		return reinterpret_cast<Ty_*>(rawData.get());
	}

	std::unique_ptr<Ty_> rawData;
};

template<typename T>
using Asset = std::shared_ptr<TResource<T>>;

template<typename T>
using AssetReference = std::weak_ptr<TResource<T>>;

class ResourceManager {
public:
	using raw_text = const char*;
	// ------------------------------------------------------------------------
	/*! Get Resource
	*
	*   Gets a Loaded resource, if not loaded load it
	*/ // --------------------------------------------------------------------
	template<typename Ty_>
	DONTDISCARD std::shared_ptr<TResource<Ty_>> GetResource(raw_text name) noexcept {
		return std::reinterpret_pointer_cast<TResource<Ty_>>(GetResource(name));
	}

	DONTDISCARD std::shared_ptr<IResource> GetResource(raw_text name);
	void RemoveResource(raw_text name) noexcept;
	void Initialize();
	DONTDISCARD IResourceImporter* GetImporterByExtension(raw_text ext) const noexcept;
	void AddImporter(IResourceImporter* res, raw_text ext) noexcept;
	void ShutDown();

	template<typename T>
	std::shared_ptr<std::string> GetResourceName(const Asset<T>& asset);

protected:
	std::shared_ptr<IResource> AddResource(raw_text mPath);

	std::unordered_map<std::string, std::shared_ptr<IResource>> resources;
	std::unordered_map<std::string, IResourceImporter*> importers;
};



template<typename T>
std::shared_ptr<std::string> ResourceManager::GetResourceName(const Asset<T>& asset) {
	for (auto a : resources) {
		std::cout << a.first << std::endl;
		if (a.second == asset) {
			std::cout << "Targed adquired" << std::endl;
			std::shared_ptr<std::string> string = std::make_shared<std::string>(a.first);
			return string;
		}
	}
	return NULL;
}

#endif