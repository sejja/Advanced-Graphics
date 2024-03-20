//
//	PageAllocator.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 19/05/21
//	Copyright © 2024. All Rights reserved
//

#ifndef _PAGE_ALLOCATOR__H_
#define _PAGE_ALLOCATOR__H_

#include "CommonDefines.h"
#include "ObjectAllocator.h"

namespace Core::Memory {
	template<typename TYPE, std::size_t ALIGN = 8>
	class PageAllocator {
#pragma region //Declarations
	public:

		template<typename, std::size_t> friend class PageAllocator;

		using value_type = TYPE;
		typedef TYPE value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef TYPE* pointer;
		typedef const TYPE* const_pointer;
		typedef TYPE& reference;
		typedef const TYPE& const_reference;
		template <class U1> struct rebind { typedef PageAllocator<U1> other; };
#pragma endregion

#pragma region //Constructors & Destructor
		PageAllocator();
		PageAllocator(const PageAllocator& other);
		template <class U1> PageAllocator(const PageAllocator<U1>& other);
		~PageAllocator();
#pragma endregion

#pragma region //Functions
		DONTDISCARD constexpr TYPE inline* allocate(const size_t size = 1) const;

		template<typename ...Args>
		DONTDISCARD constexpr TYPE* New(const size_t size = 1, Args&& ...ag) const;

		constexpr void deallocate(TYPE* const data) const;

		template<typename ...Args>
		void inline construct(TYPE* const data, Args&& ... ag) const;

		void inline destroy(TYPE* const data) const;
		void terminate(TYPE* const data) const;
#pragma endregion

#pragma region //Members
	private:
		Core::Memory::ObjectAllocator<TYPE, 64, ALIGN>* mOA;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Fetches the Original, unabstrated ObjectAllocator
	*/ //----------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	PageAllocator<TYPE, ALIGN>::PageAllocator() {
		static Core::Memory::ObjectAllocator<TYPE, 64, ALIGN> tfne;
		mOA = &tfne;
	}

	// ------------------------------------------------------------------------
	/*! Copy Constructor
	*
	*   Fetches the Original, unabstrated ObjectAllocator
	*/ //----------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	PageAllocator<TYPE, ALIGN>::PageAllocator(const PageAllocator& other) :
		mOA(other.mOA) {}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees the exceeding memory
	*/ //----------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	PageAllocator<TYPE, ALIGN>::~PageAllocator() {
		mOA->FreeEmptyPages();
	}

	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   Allocates sizeof(TYPE) * size bytes
	*/ //--------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	constexpr TYPE inline* PageAllocator<TYPE, ALIGN>::allocate(size_t) const {
		return reinterpret_cast<TYPE*>(mOA->Allocate());
	}

	// ------------------------------------------------------------------------
	/*! New
	*
	*   Allocates an construct new memory
	*/ //----------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	template<typename ...Args>
	constexpr DONTDISCARD TYPE* PageAllocator<TYPE, ALIGN>::New(const size_t size, Args&& ...ag) const {
		pointer _temp = allocate(size);
		construct(_temp, ag...);
		return _temp;
	}

	// ------------------------------------------------------------------------
	/*! Deallocate
	*
	*   Deallocates the passed chunk
	*/ //-------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	constexpr void PageAllocator<TYPE, ALIGN>::deallocate(TYPE* const data) const {
		static size_t dealloccount = 0;

		if (data) mOA->Deallocate(reinterpret_cast<void*>(data));
		dealloccount++;

		if (dealloccount >= 64) {
			mOA->FreeEmptyPages();
			dealloccount = 0;
		}
	}

	// ------------------------------------------------------------------------
	/*! Construct
	*
	*   construct a variable of type TYPE
	*/ //-------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	template<typename ...Args>
	void PageAllocator<TYPE, ALIGN>::construct(TYPE* const data, Args&& ...ag) const {
		new (data) TYPE(ag...);
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Calls the TYPE destructor
	*/ //-------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	void PageAllocator<TYPE, ALIGN>::destroy(TYPE* const data) const {
		data->~TYPE();
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Calls the TYPE destructor
	*/ //-------------------------------------------------------------------
	template<typename TYPE, std::size_t ALIGN>
	void PageAllocator<TYPE, ALIGN>::terminate(TYPE* const data) const {
		destroy(data);
		deallocate(data);
	}
}

#endif