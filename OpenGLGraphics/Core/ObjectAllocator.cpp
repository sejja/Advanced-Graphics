//
//	ObjectAllocator.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 . All Rights reserved
//
#include "ObjectAllocator.h"
#include <cassert>

namespace Memory {
		/**
* Constructs an object allocator class with an object size and a predefined configuration
* @param object_size	The size of every object
* @param config			The predefined configuration
* @return
*/
	object_allocator::object_allocator(size_t object_size, oa_config const& config) :
		mConfig(config), mHeadBlock(nullptr), mFreeList(nullptr) {
		mStat.m_object_size = object_size;

		//Configuration Alignment
		if (mConfig.mAlignment > 1) {
			mConfig.mInterAlignSize = mConfig.mAlignment -
				(object_size + mConfig.mHeaderBlockInfo.mSize)
				% mConfig.mAlignment;
			mConfig.mLeftAlignSize = mConfig.mAlignment - (
				mConfig.mHeaderBlockInfo.mSize + sizeof(void*)) % mConfig.mAlignment;
		}

		mStat.m_page_size = sizeof(generic_object*) + mConfig.mLeftAlignSize + mConfig.mObjectsPerPage *
			(object_size + mConfig.mHeaderBlockInfo.mSize)
			+ mConfig.mInterAlignSize * (mConfig.mObjectsPerPage - 1);
		allocate_new_page_();
	}

	/**
	* Destructs an object allocator. Ensures no memory leaks
	*/
	object_allocator::~object_allocator() {
		//while there are headers to free
		while (generic_object* iterator = mHeadBlock) {
			mHeadBlock = mHeadBlock->next;
			delete[] iterator;
		}
	}

	/**
	* Allocates a new memory block in one of our pages
	* @param label	The object label
	* @return		Pointer to the allocated block
	*/
	void* object_allocator::allocate(char const* label) {
		generic_object* allocated_ = nullptr;

		//If we do have Free Objects
		if (!mStat.m_free_objects || !mFreeList)
			allocate_new_page_();

		mStat.m_free_objects--;
		allocated_ = mFreeList;

		mFreeList = mFreeList->next;
		header_increase_use_count(allocated_, label);

		return allocated_;
	}

	/**
	* Deallocates a block of memory deallocated by the object allocator
	* @param object	The allocated object to free
	*/
	void object_allocator::deallocate(void* object) {
		assert(object);

		reinterpret_cast<generic_object*>(object)->next = mFreeList;
		mFreeList = reinterpret_cast<generic_object*>(object);
		header_decrease_use_count(reinterpret_cast<generic_object*>(object));

		mStat.m_free_objects++;
	}

	/**
	* Frees all the unused empty pages
	* @return	The number of freed pages
	*/
	unsigned object_allocator::free_empty_pages() {
		unsigned freedPages_ = 0;

		const auto fpurge_page_ = [&](generic_object* it) -> void {
			purge_page_from_freelist(it);
			delete[] it;
			freedPages_++;
		};

		//First, delete all the unused pages starting from the end (to place the Head Block pointer correctly)
		for (generic_object* iterator = mHeadBlock; iterator && is_page_unused(iterator);) {
			mHeadBlock = iterator->next;
			fpurge_page_(iterator);
			iterator = mHeadBlock;
		}

		//Now, delete the rest of the empty pages pages
		for (generic_object* iterator = mHeadBlock, *previousit = iterator; iterator;) {
			//If the page is used
			if (is_page_unused(iterator)) {
				previousit->next = iterator->next;
				fpurge_page_(iterator);
				iterator = previousit->next;
				continue;
			}

			previousit = iterator;
			iterator = iterator->next;
		}

		mStat.m_pages_in_use -= freedPages_;

		return freedPages_;
	}

	/**
	* Allocates and sets a new page
	*/
	void object_allocator::allocate_new_page_() {
		generic_object* temp_;
		mStat.m_pages_in_use++;

		try { temp_ = reinterpret_cast<generic_object*>(new char[mStat.m_page_size]); }
		catch (...) { throw oa_exception(oa_exception::OA_EXCEPTION::E_NO_MEMORY, "Keyword new raised an Exception"); }

		configure_headers(temp_);
		temp_->next = mHeadBlock;
		mHeadBlock = temp_;

		//Add all the new objects to the free list
		for (size_t i = 0; i < mConfig.mObjectsPerPage; i++)
			mFreeList = set_page_ptr_at_index(temp_, i, mFreeList);

		mStat.m_free_objects = mConfig.mObjectsPerPage;
	}

	/**
	* Returns a pointer to an object given the page and the index
	* @param page	The pointer to the page
	* @param index	The index of the object
	* @return	The pointer to the wanted object
	*/
	object_allocator::generic_object inline* object_allocator::get_object_ptr(generic_object* page, size_t index)
		const noexcept {
		return reinterpret_cast<generic_object*>(reinterpret_cast<char*>(page) + sizeof(void*) +
			mConfig.mLeftAlignSize +
			((mStat.m_object_size + mConfig.mHeaderBlockInfo.mSize
				+ mConfig.mInterAlignSize)
				* index + mConfig.mHeaderBlockInfo.mSize));
	}

	/**
	* Sets the pointer to an unused object on a page and returns the pointer to the destiny object
	* @param page	The pointer to the page
	* @param index	The index of the object
	* @param ptr	The pointer value to use
	* @return	The pointer to the wanted object
	*/
	object_allocator::generic_object* object_allocator::set_page_ptr_at_index(generic_object* page, size_t index,
		generic_object* ptr) noexcept {
		generic_object* dst_ = get_object_ptr(page, index);
		dst_->next = ptr;

		return dst_;
	}

	/**
	* Sets the object allcocation headers for all the objects
	* @param page	The pointer to the page
	*/
	void object_allocator::configure_headers(generic_object* page) noexcept {
		//For each header on the page
		for (size_t i = 0; i < mConfig.mObjectsPerPage; i++)
			memset(get_object_header(get_object_ptr(page, i)), 0, mConfig.mHeaderBlockInfo.mSize);
	}

	/**
	* Checks if a certain object is allocated
	* @param object	The pointer to the object
	* @return	Is it allocated?
	*/
	bool object_allocator::is_object_allocated(generic_object* object) const noexcept {
		return *(reinterpret_cast<char*>(object) - 1);
	}

	/**
	* Increase the memory object count on it's header
	* @param object	The pointer to the object
	* @param label	The object label
	*/
	void inline object_allocator::header_increase_use_count(generic_object* object, const char* label) noexcept {
		*(reinterpret_cast<char*>(object) - 1) = 1;
	}

	/**
	* Decrease the memory object count on it's header
	* @param object	The pointer to the object
	*/
	void inline object_allocator::header_decrease_use_count(generic_object* object) noexcept {
		*(reinterpret_cast<char*>(object) - 1) = 0;
	}

	/**
	* Returns the Header of an object
	* @param object	The pointer to the object
	* @return	The pointer to the object's header
	*/
	void inline* object_allocator::get_object_header(generic_object* object) const noexcept {
		return reinterpret_cast<char*>(object) - mConfig.mHeaderBlockInfo.mSize;
	}

	/**
	* Returns wether a certain page is unused
	* @param page	The pointer to the page
	* @return	unused?
	*/
	bool object_allocator::is_page_unused(generic_object* page) const noexcept {
		//Iterate through every object of the page
		for (size_t i = 0; i < mConfig.mObjectsPerPage; i++) {
			generic_object* obj_ = get_object_ptr(page, i);

			//if the object is allocated
			if (is_object_allocated(obj_))
				return false;
		}

		return true;
	}

	/**
	* Removes every reference of objects that are within a page from the free list
	* @param page	The pointer to the page
	*/
	void object_allocator::purge_page_from_freelist(generic_object* page) noexcept {
		//Iterate through the free list
		for (generic_object* iterator = mFreeList; iterator;) {
			//If the object is on the free list, remove it
			if (iterator >= page && iterator < page + mStat.m_page_size) {
				mFreeList = iterator = iterator->next;
				continue;
			}

			iterator = iterator->next;
		}

		mStat.m_free_objects -= mConfig.mObjectsPerPage;
	}
}