//
//	ObjectAllocator.h
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef OBJECTALLOCATORH
#define OBJECTALLOCATORH

#include <string>

namespace Memory {
	class object_allocator {
	public:
		struct oa_config {
			struct header_block_info {
				size_t  mSize;

				header_block_info() : mSize(1) {
				};
			};

			oa_config(unsigned ObjectsPerPage = 64,
				header_block_info const& HBInfo = header_block_info(),
				unsigned Alignment = 8) :
				mObjectsPerPage(ObjectsPerPage),
				mHeaderBlockInfo(HBInfo),
				mAlignment(Alignment)
			{
				mLeftAlignSize = 0;
				mInterAlignSize = 0;
			}

			unsigned mObjectsPerPage;
			header_block_info mHeaderBlockInfo;
			unsigned mAlignment;
			unsigned mLeftAlignSize;
			unsigned mInterAlignSize;
		};

		class oa_exception {
		public:
			enum class OA_EXCEPTION : int {
				E_NO_MEMORY,
				E_NO_PAGES,
				E_BAD_BOUNDARY,
				E_MULTIPLE_FREE,
				E_CORRUPTED_BLOCK
			};

			oa_exception(OA_EXCEPTION ErrCode, std::string const& Message) :
				m_error_code(ErrCode), m_message(Message) {};

			virtual ~oa_exception() = default;

			OA_EXCEPTION        code() const { return m_error_code; }
			virtual char const* what() const { return m_message.c_str(); }

		private:
			OA_EXCEPTION m_error_code;
			std::string  m_message;
		};

		struct oa_stats {
			oa_stats(void) :
				m_object_size(0), m_page_size(0), m_free_objects(0), m_pages_in_use(0)
			{};

			size_t   m_object_size;
			size_t   m_page_size;
			unsigned m_free_objects;
			unsigned m_pages_in_use;
		};

		struct generic_object
		{
			generic_object* next;
		};

		object_allocator(size_t object_size, oa_config const& config);
		object_allocator(object_allocator const& oa) = delete;
		object_allocator& operator=(object_allocator const& oa) = delete;
		~object_allocator();
		void* allocate(char const* label = 0);
		void deallocate(void* object);
		unsigned free_empty_pages();

	private:
		void allocate_new_page_();
		void configure_headers(generic_object* page) noexcept;
		bool is_object_allocated(generic_object* object) const noexcept;
		void inline header_increase_use_count(generic_object* object,
			const char* label) noexcept;
		void inline header_decrease_use_count(generic_object* object) noexcept;
		generic_object inline* get_object_ptr(generic_object* page,
			size_t object) const noexcept;
		generic_object* set_page_ptr_at_index(generic_object* page, size_t index,
			generic_object* ptr) noexcept;
		void inline* get_object_header(generic_object* object) const noexcept;
		bool is_page_unused(generic_object* page) const noexcept;
		void purge_page_from_freelist(generic_object* pageAddr) noexcept;

		oa_config mConfig;
		oa_stats mStat;
		generic_object* mHeadBlock;
		generic_object* mFreeList;
	};
}

#endif