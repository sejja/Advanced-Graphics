//
//  SharedResources.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 8/4/24.
//  Copyright � 2024 . All rights reserved.
//

#ifndef _RESOURCE_MANAGER__H_
#define _RESOURCE_MANAGER__H_

#include "CommonDefines.h"
#include <string>
#include <memory>
#include <unordered_map>
#include "Core/Singleton.h"

namespace Core {
	namespace Assets {
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
			TResource(Ty_* const data);
			~TResource();
			DONTDISCARD Ty_ inline* Get() noexcept;
			DONTDISCARD Ty_ inline* GetAndRelease();

		private:
			std::unique_ptr<Ty_> mRawData;
		};

		template<typename T>
		using Asset = std::shared_ptr<TResource<T>>;

		template<typename T>
		using AssetReference = std::weak_ptr<TResource<T>>;

		class ResourceManager {
		public:
			using raw_text = const char*;

			template<typename Ty_>
			DONTDISCARD Asset<Ty_> GetResource(raw_text name);
			DONTDISCARD std::shared_ptr<IResource> GetResource(raw_text name);
			DONTDISCARD IResourceImporter* GetImporterByExtension(raw_text ext) const;
			void RemoveResource(raw_text name);
			void Initialize();
			void ShutDown();

		protected:
			std::shared_ptr<IResource> AddResource(raw_text mPath);
			std::unordered_map<std::string, std::shared_ptr<IResource>> mResources;
			std::unordered_map<std::string, std::unique_ptr<IResourceImporter>> mImporters;
		};
		
		// ------------------------------------------------------------------------
		/*! Copy Constructor
		*
		*   Creates a TResource from a <Ty_> pointer
		*/ // ---------------------------------------------------------------------
		template<typename Ty_>
		TResource<Ty_>::TResource(Ty_* const data) :
			mRawData(data) {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION - Used for polymorphism
		*/ // ---------------------------------------------------------------------
		template<typename Ty_>
		TResource<Ty_>::~TResource() {}
		
		// ------------------------------------------------------------------------
		/*! Get
		*
		*   Gets the Resource as something
		*/ // ---------------------------------------------------------------------
		template<typename Ty_>
		Ty_* TResource<Ty_>::Get() noexcept {
			return reinterpret_cast<Ty_*>(mRawData.get());
		}

		// ------------------------------------------------------------------------
		/*! Get & Release
		*
		*   Removes ownership of the data
		*/ // ---------------------------------------------------------------------
		template<typename Ty_>
		Ty_* TResource<Ty_>::GetAndRelease() {
			return reinterpret_cast<Ty_*>(mRawData.release());
		}

		// ------------------------------------------------------------------------
		/*! Get Resource
		*
		*   Gets a Loaded resource, if not loaded load it
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		Asset<Ty_> ResourceManager::GetResource(raw_text name) {
			return std::reinterpret_pointer_cast<TResource<Ty_>>(GetResource(name));
		}
	}
}

#endif