// src\Tool\HeapMem.h - allocation tiny code, prepared for unittest
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { namespace Tool { 
class HeapMem {
	mutable size_t m_cbAllocSize;
	mutable LPBYTE m_lpMem;
	LPBYTE alloc_(size_t cbAllocSize) const {
		auto p = ::HeapAlloc( ::GetProcessHeap( ), HEAP_ZERO_MEMORY, cbAllocSize );
		return static_cast<LPBYTE>( p );
	}
public:
	// Its "MAX_VALUE_NAME 16383" for registry
	static const size_t c_uDefaultAllocSize = 16383;
	explicit HeapMem(size_t cbAllocSize = c_uDefaultAllocSize) 
		: m_cbAllocSize( cbAllocSize )
		, m_lpMem( alloc_( cbAllocSize ) )
	{}
	~HeapMem() {
		::HeapFree( ::GetProcessHeap( ), 0, m_lpMem );
	}
	void *getMemPtr() const {
		return m_lpMem;
	}
	size_t getSize() const {
		return m_cbAllocSize;
	}

	// TEST
	HeapMem(const HeapMem &rhs) 
		: m_cbAllocSize ( rhs.m_cbAllocSize )
		, m_lpMem( alloc_( rhs.m_cbAllocSize ) )
	{ 
		for ( size_t i = 0; i < m_cbAllocSize; ++i )
			m_lpMem[ i ] = rhs.m_lpMem[ i ];
	}
	HeapMem& operator=(const HeapMem &rhs) {
		std::swap( m_cbAllocSize, rhs.m_cbAllocSize );
		std::swap( m_lpMem, rhs.m_lpMem );
		return *this;
	}
};
}}} // namespace prj_sysw::TinySynapticsScroll::Tool _
