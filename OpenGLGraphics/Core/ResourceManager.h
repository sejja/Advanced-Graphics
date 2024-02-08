//
//  SharedResources.h
//  Good Neighbours
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
#include "Allocator.h"
#include "Singleton.h"
#include "Graphics/Primitives/Shader.h"

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
	TResource() = default;
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

	std::unique_ptr<Ty_> rawData = nullptr;
};

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

protected:
	std::shared_ptr<IResource> AddResource(raw_text mPath);

	std::unordered_map<std::string, std::shared_ptr<IResource>> resources;
	std::unordered_map<std::string, IResourceImporter*> importers;
};

template<typename T>
using Asset = std::shared_ptr<TResource<T>>;

template<typename T>
using AssetReference = std::weak_ptr<TResource<T>>;

#endif