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
		#pragma region //Destrucor
			virtual ~IResource();
		#pragma endregion
		};

		class IResourceImporter {
		#pragma region //Functions
		public:
			virtual std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const = 0;
		#pragma endregion
		};

		template<typename Ty_>
		class TResource : public IResource {
		#pragma region //Constructors
		public:
			TResource(Ty_* const data);
			~TResource();
		#pragma endregion

		#pragma region //Functions
			DONTDISCARD Ty_ inline* Get() noexcept;
			DONTDISCARD Ty_ inline* GetAndRelease();
		#pragma endregion

		#pragma region //Members
		private:
			std::unique_ptr<Ty_> mRawData;
		#pragma endregion
		};

		template<typename T>
		using Asset = std::shared_ptr<TResource<T>>;

		template<typename T>
		using AssetReference = std::weak_ptr<TResource<T>>;

		class ResourceManager {
		#pragma region //Declarations
		public:
			CLASS_EXCEPTION(ResourceManager)
			using raw_text = const char*;
		#pragma endregion

		#pragma region //Functions
			template<typename Ty_>
			DONTDISCARD Asset<Ty_> GetResource(raw_text name);
			DONTDISCARD std::shared_ptr<IResource> GetResource(raw_text name);
			DONTDISCARD std::shared_ptr<IResourceImporter> GetImporterByExtension(raw_text ext) const;
			void RemoveResource(raw_text name);
			void Initialize();
			template<typename T>
			std::shared_ptr<std::string> GetResourceName(const Asset<T>& asset);
		#pragma endregion

		#pragma region //Members
		protected:
			std::shared_ptr<IResource> AddResource(raw_text mPath);
			std::unordered_map<std::string, std::shared_ptr<IResource>> mResources;
			std::unordered_map<std::string, std::shared_ptr<IResourceImporter>> mImporters;
		#pragma endregion
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

		template<typename T>
		std::shared_ptr<std::string> ResourceManager::GetResourceName(const Asset<T>& asset) {
			for (auto a : mResources) {
				//std::cout << a.first << std::endl;
				if (a.second == asset) {
					//std::cout << "Targed adquired" << std::endl;
					std::shared_ptr<std::string> string = std::make_shared<std::string>(a.first);
					return string;
				}
			}
			return NULL;
		}
	}
}

#endif