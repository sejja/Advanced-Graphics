//
//	PageAllocator.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 19/05/21
//	Copyright © 2021. All Rights reserved
//

#ifndef _PAGE_ALLOCATOR__H_
#define _PAGE_ALLOCATOR__H_

#include "../CommonDefines.h"
#include "ObjectAllocator.h"

template<typename TYPE>
class PageAllocator {
public:

	template<typename> friend class PageAllocator;

	using value_type = TYPE;
	typedef TYPE value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef TYPE* pointer;
	typedef const TYPE* const_pointer;
	typedef TYPE& reference;
	typedef const TYPE& const_reference;
	template <class U1> struct rebind { typedef PageAllocator<U1> other; };
	PageAllocator(void) {
		static Memory::object_allocator
			galloc(sizeof(TYPE), Memory::object_allocator::oa_config());
		mOA = &galloc;
	};
	PageAllocator(const PageAllocator& other) : mOA(other.mOA) {};
	template <class U1> PageAllocator(const PageAllocator<U1>& other) : mOA(sizeof(TYPE), other.mOA.config()) {};
	DONTDISCARD constexpr TYPE inline* allocate(const size_t size = 1) const noexcept;
	template<typename ...Args>
	DONTDISCARD constexpr TYPE inline* New(const size_t size = 1, Args ...ag) const noexcept;
	void inline deallocate(TYPE* data) const noexcept;
	template<typename ...Args>
	void inline construct(TYPE* data, Args... ag) const noexcept;
	void inline destroy(TYPE* data) noexcept;
private:
	Memory::object_allocator* mOA;
};

// ------------------------------------------------------------------------
/*! Allocate
*
*   Allocates sizeof(TYPE) * size bytes
*/ //--------------------------------------------------------------------
template<typename TYPE>
constexpr TYPE inline* PageAllocator<TYPE>::allocate(size_t) const noexcept {
	return reinterpret_cast<TYPE*>(mOA->allocate());
}

// ------------------------------------------------------------------------
/*! New
*
*   Allocates an construct new memory
*/ //----------------------------------------------------------------------
template<typename TYPE>
template<typename ...Args>
inline constexpr DONTDISCARD TYPE* PageAllocator<TYPE>::New(const size_t size, Args ...ag) const noexcept {
	pointer _temp = allocate(size);
	construct(_temp, ag...);
	return _temp;
}

// ------------------------------------------------------------------------
/*! Deallocate
*
*   Deallocates the passed chunk
*/ //-------------------------------------------------------------------
template<typename TYPE>
void PageAllocator<TYPE>::deallocate(TYPE* data) const noexcept {
	static size_t dealloccount = 0;

	if (data) mOA->deallocate(data);
	dealloccount++;

	if (dealloccount >= 64) {
		mOA->free_empty_pages();
		dealloccount = 0;
	}
}

// ------------------------------------------------------------------------
/*! Construct
*
*   construct a variable of type TYPE
*/ //-------------------------------------------------------------------
template<typename TYPE>
template<typename ...Args>
inline void PageAllocator<TYPE>::construct(TYPE* data, Args ...ag) const noexcept {
	new (data) TYPE(ag...);
}

// ------------------------------------------------------------------------
/*! Destroy
*
*   Calls the TYPE destructor
*/ //-------------------------------------------------------------------
template<typename TYPE>
void inline PageAllocator<TYPE>::destroy(TYPE* data) noexcept {
	data->~TYPE();
}

#endif