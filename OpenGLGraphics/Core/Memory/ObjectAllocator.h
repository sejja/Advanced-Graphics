//
//	ObjectAllocator.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 19/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _OBJECT_ALLOCATOR__H_
#define _OBJECT_ALLOCATOR__H_

#include <string_view>
#include <cassert> 
#include "CommonDefines.h"

namespace Core {
	namespace Memory {
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		class ObjectAllocator {
#pragma region //Declarations
		public:
			CLASS_EXCEPTION(ObjectAllocator)

			struct ObjectAllocatorConfiguration {
			#pragma region //Declarations
				struct HeaderBlockInfo {
				#pragma region //Constructor
					HeaderBlockInfo() noexcept;
				#pragma endregion

				#pragma region //Members
					std::size_t  mSize;
				#pragma endregion
				};
			#pragma endregion

			#pragma region //Constructor
				ObjectAllocatorConfiguration(HeaderBlockInfo const& HBInfo = HeaderBlockInfo()) noexcept;
			#pragma endregion

			#pragma region //Functions
				DONTDISCARD inline constexpr std::size_t GetAlignmentBits() const noexcept;
				DONTDISCARD inline constexpr std::size_t ObjectsPerPage() const noexcept;
				DONTDISCARD constexpr inline HeaderBlockInfo GetInfo() const noexcept;
				DONTDISCARD inline constexpr std::size_t GetLeftAlignmentBits() const noexcept;
				DONTDISCARD inline constexpr std::size_t GetInterAlignmentBits() const noexcept;
				constexpr inline void SetInterAlignmentBits(const std::size_t alignment) noexcept;
				constexpr inline void SetLeftAlignmentBits(const std::size_t alignment) noexcept;
			#pragma endregion

			#pragma region //Members
			private:
				std::size_t mInterAlignSize, mLeftAlignSize;
				HeaderBlockInfo mHeaderBlockInfo;
			#pragma endregion
			};

			struct ObjectAllocatorStatistics {
			#pragma region //Constructor
				ObjectAllocatorStatistics() noexcept;
			#pragma endregion

			#pragma region //Members
				std::size_t mPageSize;
				std::size_t mFreeObjects;
				std::size_t mUsedPages;
			#pragma endregion
			};

			struct GenericObjectNode {
			#pragma region //Members
				GenericObjectNode* mNext;
			#pragma endregion
			};

#pragma region //Constructor & Destructor
			ObjectAllocator();
			ObjectAllocator(ObjectAllocator const& oa) = delete;
			ObjectAllocator& operator=(ObjectAllocator const& oa) = delete;
			~ObjectAllocator();
#pragma endregion

#pragma region //Functions
			void* Allocate();
			void Deallocate(void* object);
			unsigned FreeEmptyPages();

		private:
			void AllocateNewPage();
			void ConfigureHeaders(GenericObjectNode* page) noexcept;
			bool IsObjectAllocated(GenericObjectNode* object) const noexcept;
			void inline HeaderIncreaseUseCount(GenericObjectNode* object) noexcept;
			void inline HeaderDecreaseUseCount(GenericObjectNode* object) noexcept;
			void inline* GetObjectHeader(GenericObjectNode* object) const noexcept;
			bool IsPageUnused(GenericObjectNode* page) const noexcept;
			void PurgePageFromFreelist(GenericObjectNode* pageAddr) noexcept;

			// ------------------------------------------------------------------------
			/*! Get Object Pointer
			*
			*   Returns a pointer to an object given the page and the index
			*/ // ---------------------------------------------------------------------
			GenericObjectNode inline* GetObjectPtr(GenericObjectNode* page, size_t object) const noexcept {
				return reinterpret_cast<GenericObjectNode*>(reinterpret_cast<char*>(page) + sizeof(void*) +
					mConfig.GetLeftAlignmentBits() +
					((sizeof(T) + mConfig.GetInfo().mSize
						+ mConfig.GetInterAlignmentBits())
						* object + mConfig.GetInfo().mSize));
			};

			// ------------------------------------------------------------------------
			/*! Set Page Pointer At Index
			*
			*   Sets the pointer to an unused object on a page and returns the pointer 
			*		to the destiny object
			*/ // ---------------------------------------------------------------------
			GenericObjectNode* SetPagePtrAtIndex(GenericObjectNode* page, size_t index,
				GenericObjectNode* ptr) noexcept {
				GenericObjectNode* dst_ = GetObjectPtr(page, index);
				dst_->mNext = ptr;

				return dst_;
			};

#pragma endregion

#pragma region //Members
			ObjectAllocatorConfiguration mConfig;
			ObjectAllocatorStatistics mStat;
			GenericObjectNode* mHeadBlock;
			GenericObjectNode* mFreeList;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Sets the Size to 1
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::HeaderBlockInfo::HeaderBlockInfo() noexcept
			: mSize(1) {}

		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Takes a reference to another Configuration object
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::ObjectAllocatorConfiguration(HeaderBlockInfo const& HBInfo) noexcept
			: mHeaderBlockInfo(HBInfo), mLeftAlignSize(0), mInterAlignSize(0) {}

		// ------------------------------------------------------------------------
		/*! Objects per Page
		*
		*   Returns the count of objects per page
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr std::size_t ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::GetAlignmentBits() const noexcept {
			return OBJECTS;
		}

		// ------------------------------------------------------------------------
		/*! Get Alingment Bits
		*
		*   Returns the number of bits to align with
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr std::size_t ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::ObjectsPerPage() const noexcept {
			return ALIGN;
		}

		// ------------------------------------------------------------------------
		/*! Get Inter-Alingment bits
		*
		*   Gets the number of bits of free memory between elements in a page
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr void ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::SetInterAlignmentBits(const std::size_t alignment) noexcept {
			mInterAlignSize = alignment;
		}

		// ------------------------------------------------------------------------
		/*! Get Inter-Alingment bits
		*
		*   Returns the Internal alignment
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr std::size_t ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::GetInterAlignmentBits() const noexcept {
			return mInterAlignSize;
		}

		// ------------------------------------------------------------------------
		/*! Get Left-Alingment bits
		*
		*   Returns the Left alignment of a page
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr std::size_t ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::GetLeftAlignmentBits() const noexcept {
			return mLeftAlignSize;
		}

		// ------------------------------------------------------------------------
		/*! Get Left Alignment bits
		*
		*   Gets the alignment bits at the left of the page
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr void ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::SetLeftAlignmentBits(const std::size_t alignment) noexcept {
			mLeftAlignSize = alignment;
		}

		// ------------------------------------------------------------------------
		/*! Get Info
		*
		*   Guess what this function does (Exercise left to the reader)
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		constexpr ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::HeaderBlockInfo ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorConfiguration::GetInfo() const noexcept {
			return mHeaderBlockInfo;
		}

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs an object allocator class with an object size and a predefined configuration
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocator() :
			mHeadBlock(nullptr), mFreeList(nullptr),
			mConfig(ObjectAllocatorConfiguration()) {

			//Configuration Alignment
			if (mConfig.GetAlignmentBits() > 1) {
				mConfig.SetInterAlignmentBits(mConfig.GetAlignmentBits() -
					(sizeof(T) + mConfig.GetInfo().mSize)
					% mConfig.GetAlignmentBits());
				mConfig.SetLeftAlignmentBits(mConfig.GetAlignmentBits() - (
					mConfig.GetInfo().mSize + sizeof(void*)) % mConfig.GetAlignmentBits());
			}

			mStat.mPageSize = sizeof(GenericObjectNode*) + mConfig.GetLeftAlignmentBits() + mConfig.ObjectsPerPage() *
				(sizeof(T) + mConfig.GetInfo().mSize)
				+ mConfig.GetInterAlignmentBits() * (mConfig.ObjectsPerPage() - 1);
			AllocateNewPage();
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Destructs an object allocator. Ensures no memory leaks
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		ObjectAllocator<T, OBJECTS, ALIGN>::~ObjectAllocator() {
			//while there are headers to free
			while (GenericObjectNode* iterator = mHeadBlock) {
				mHeadBlock = mHeadBlock->mNext;
				delete[] iterator;
			}
		}

		// ------------------------------------------------------------------------
		/*! Allocate
		*
		*   Allocates a new memory block in one of our pages
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void* ObjectAllocator<T, OBJECTS, ALIGN>::Allocate() {
			GenericObjectNode* allocated_ = nullptr;

			//If we do have Free Objects
			if (!mStat.mFreeObjects || !mFreeList) AllocateNewPage();

			mStat.mFreeObjects--;
			allocated_ = mFreeList;
			mFreeList = mFreeList->mNext;
			HeaderIncreaseUseCount(allocated_);

			return allocated_;
		}

		// ------------------------------------------------------------------------
		/*! Deallocate
		*
		*   Deallocates a block of memory deallocated by the object allocator
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void ObjectAllocator<T, OBJECTS, ALIGN>::Deallocate(void* object) {
			assert(object);

			reinterpret_cast<GenericObjectNode*>(object)->mNext = mFreeList;
			mFreeList = reinterpret_cast<GenericObjectNode*>(object);
			HeaderDecreaseUseCount(reinterpret_cast<GenericObjectNode*>(object));
			mStat.mFreeObjects++;
		}

		// ------------------------------------------------------------------------
		/*! Free Empty Pages
		*
		*   Frees all the unused empty pages
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		unsigned ObjectAllocator<T, OBJECTS, ALIGN>::FreeEmptyPages() {
			unsigned freedPages_ = 0;

			const auto fpurge_page_ = [&](GenericObjectNode* it) -> void {
				PurgePageFromFreelist(it);
				delete[] it;
				freedPages_++;
				};

			//First, delete all the unused pages starting from the end (to place the Head Block pointer correctly)
			for (GenericObjectNode* iterator = mHeadBlock; iterator && IsPageUnused(iterator);) {
				mHeadBlock = iterator->mNext;
				fpurge_page_(iterator);
				iterator = mHeadBlock;
			}

			//Now, delete the rest of the empty pages pages
			for (GenericObjectNode* iterator = mHeadBlock, *previousit = iterator; iterator;) {
				//If the page is used
				if (IsPageUnused(iterator)) {
					previousit->mNext = iterator->mNext;
					fpurge_page_(iterator);
					iterator = previousit->mNext;
					continue;
				}

				previousit = iterator;
				iterator = iterator->mNext;
			}

			mStat.mUsedPages -= freedPages_;

			return freedPages_;
		}

		// ------------------------------------------------------------------------
		/*! Allocate New Page
		*
		*   Allocates and sets a new page
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void ObjectAllocator<T, OBJECTS, ALIGN>::AllocateNewPage() {
			GenericObjectNode* temp_;
			mStat.mUsedPages++;

			try { temp_ = reinterpret_cast<GenericObjectNode*>(new char[mStat.mPageSize]); }
			catch (...) { throw ObjectAllocatorException("Keyword new raised an Exception"); }

			ConfigureHeaders(temp_);
			temp_->mNext = mHeadBlock;
			mHeadBlock = temp_;

			//Add all the new objects to the free list
			for (size_t i = 0; i < mConfig.ObjectsPerPage(); i++)
				mFreeList = SetPagePtrAtIndex(temp_, i, mFreeList);

			mStat.mFreeObjects = mConfig.ObjectsPerPage();
		}

		// ------------------------------------------------------------------------
		/*! Configure Headers
		*
		*   Sets the object allcocation headers for all the objects
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void ObjectAllocator<T, OBJECTS, ALIGN>::ConfigureHeaders(GenericObjectNode* page) noexcept {
			//For each header on the page
			for (size_t i = 0; i < mConfig.ObjectsPerPage(); i++)
				memset(GetObjectHeader(GetObjectPtr(page, i)), 0, mConfig.GetInfo().mSize);
		}

		// ------------------------------------------------------------------------
		/*! Is Object Allocated
		*
		*   Checks if a certain object is allocated
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		bool ObjectAllocator<T, OBJECTS, ALIGN>::IsObjectAllocated(GenericObjectNode* object) const noexcept {
			return *(reinterpret_cast<char*>(object) - 1);
		}

		// ------------------------------------------------------------------------
		/*! Header Increase Use Count
		*
		*   Increase the memory object count on it's header
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void inline ObjectAllocator<T, OBJECTS, ALIGN>::HeaderIncreaseUseCount(GenericObjectNode* object) noexcept {
			*(reinterpret_cast<char*>(object) - 1) = 1;
		}

		// ------------------------------------------------------------------------
		/*! Header Decrease Use Count
		*
		*   Decrease the memory object count on it's header
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void inline ObjectAllocator<T, OBJECTS, ALIGN>::HeaderDecreaseUseCount(GenericObjectNode* object) noexcept {
			*(reinterpret_cast<char*>(object) - 1) = 0;
		}

		// ------------------------------------------------------------------------
		/*! Get Object Header
		*
		*   Returns the Header of an object
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void inline* ObjectAllocator<T, OBJECTS, ALIGN>::GetObjectHeader(GenericObjectNode* object) const noexcept {
			return reinterpret_cast<char*>(object) - mConfig.GetInfo().mSize;
		}

		// ------------------------------------------------------------------------
		/*! Is Page Unused
		*
		*   Returns wether a certain page is unused
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		bool ObjectAllocator<T, OBJECTS, ALIGN>::IsPageUnused(GenericObjectNode* page) const noexcept {
			//Iterate through every object of the page
			for (size_t i = 0; i < mConfig.ObjectsPerPage(); i++) {
				GenericObjectNode* obj_ = GetObjectPtr(page, i);

				//if the object is allocated
				if (IsObjectAllocated(obj_))
					return false;
			}

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Purge Page From Free List
		*
		*   Removes every reference of objects that are within a page from the free list
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		void ObjectAllocator<T, OBJECTS, ALIGN>::PurgePageFromFreelist(GenericObjectNode* page) noexcept {
			//Iterate through the free list
			for (GenericObjectNode* iterator = mFreeList; iterator;) {
				//If the object is on the free list, remove it
				if (iterator >= page && iterator < page + mStat.mPageSize) {
					mFreeList = iterator = iterator->mNext;
					continue;
				}

				iterator = iterator->mNext;
			}

			mStat.mFreeObjects -= mConfig.ObjectsPerPage();
		}

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs an Statistical Block
		*/ // ---------------------------------------------------------------------
		template<typename T, std::size_t OBJECTS, std::size_t ALIGN>
		ObjectAllocator<T, OBJECTS, ALIGN>::ObjectAllocatorStatistics::ObjectAllocatorStatistics() noexcept
			: mPageSize(0), mFreeObjects(0), mUsedPages(0) {}
	}
}

#endif