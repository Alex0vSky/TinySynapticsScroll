#include "..\pch.h"

// There is kernel32.dll!_HeapAlloc mapped/forward to ntdll.dll!_RtlAllocateHeap
struct MockMethodGlobal {
	MOCK_METHOD(LPVOID, HeapAlloc_, (HANDLE m_hHeap, DWORD dwFlags, SIZE_T dwBytes) );
	MOCK_METHOD(BOOL, HeapFree_, (HANDLE m_hHeap, DWORD dwFlags, LPVOID lpMem) );
};
::testing::NiceMock< MockMethodGlobal > *g_poMockMethodHolder;
#define HeapAlloc(a0_, a1_, a2_) \
	g_poMockMethodHolder ->HeapAlloc_( a0_, a1_, a2_ )
#define HeapFree(a0_, a1_, a2_) \
	g_poMockMethodHolder ->HeapFree_( a0_, a1_, a2_ )
#include "..\..\..\src\Tool\HeapMem.h"
#undef HeapAlloc
#undef HeapFree

using testing::_;
using ::testing::Return;
using namespace prj_sysw::TinySynapticsScroll;

MOCK_STDCALL_FUNC(HANDLE, GetProcessHeap);

namespace testMem {
class MemFixture : public ::testing::Test {
protected:
	const HANDLE m_hHeap;
	MemFixture() 
		: m_hHeap( ::GetProcessHeap( ) )
	{
		g_poMockMethodHolder = new std::decay_t< decltype( *g_poMockMethodHolder ) >;
	}
	~MemFixture() {
		delete g_poMockMethodHolder;
	}
};

typedef MemFixture CreateDestroy;
NAMESPACE_TEST_F(Tool_HeapMem, CreateDestroy, simple) {
	EXPECT_MODULE_FUNC_CALL( GetProcessHeap )
		.Times( 2 )
		.WillRepeatedly( testing::Return( m_hHeap ) );
	EXPECT_CALL( *g_poMockMethodHolder, HeapAlloc_( m_hHeap, HEAP_ZERO_MEMORY, Tool::HeapMem::c_uDefaultAllocSize ) )
		.Times( 1 );
	EXPECT_CALL( *g_poMockMethodHolder, HeapFree_( m_hHeap, 0, _ ) )
		.Times( 1 );
	{
		Tool::HeapMem oMem;
	}
	RESTORE_MODULE_FUNC( GetProcessHeap );
}

class Operate : public MemFixture {
protected:
	Operate() {
		ON_CALL( *g_poMockMethodHolder, HeapAlloc_( m_hHeap, HEAP_ZERO_MEMORY, Tool::HeapMem::c_uDefaultAllocSize ) )
			.WillByDefault( [](HANDLE m_hHeap, DWORD dwFlags, SIZE_T dwBytes) {
					return ::HeapAlloc( m_hHeap, dwFlags, dwBytes );
				}
			);
		ON_CALL( *g_poMockMethodHolder, HeapFree_( m_hHeap, 0, _ ) )
			.WillByDefault( [](HANDLE m_hHeap, DWORD dwFlags, LPVOID lpMem) {
					return ::HeapFree( m_hHeap, 0, lpMem );
				}
			);
	}
};

NAMESPACE_TEST_F(Tool_HeapMem, Operate, getMemPtr) {
	Tool::HeapMem oMem;
	EXPECT_NE( oMem.getMemPtr( ), nullptr );
}

NAMESPACE_TEST_F(Tool_HeapMem, Operate, getSize) {
	size_t cbAllocSize = 100;
	Tool::HeapMem oMem( cbAllocSize );
	EXPECT_EQ( oMem.getSize( ), cbAllocSize );
}

typedef Operate Filling;
NAMESPACE_TEST_F(Tool_HeapMem, Filling, string) {
	Tool::HeapMem oMem;
	std::string str = "Foo";
	char *sz = (char *)oMem.getMemPtr( );
	strcpy_s( sz, ( Tool::HeapMem::c_uDefaultAllocSize - 1 ), str.c_str( ) );
	EXPECT_STREQ( sz, str.c_str( ) );
}
} // namespace testMem _
