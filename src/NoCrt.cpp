// src\NoCrt.cpp - crt functionals replacement for release version. "cl /Oi- " and no "/GL"
#include "MsvcGenerated\stdafx.h"
#ifndef _DEBUG
// cause: "{}" on array/struct init
// @insp https://opensource.apple.com/source/Libc/Libc-167/string.subproj/memset.c.auto.html
_Post_equal_to_(_Dst)
_At_buffer_(
    (unsigned char*)_Dst,
    _Iter_,
    _Size,
    _Post_satisfies_(((unsigned char*)_Dst)[_Iter_] == _Val)
)
void* __cdecl memset(_Out_writes_bytes_all_(_Size) void*  _Dst, _In_ int _Val, _In_ size_t _Size) { 
    #pragma warning( push )
    #pragma warning( disable: 6001 )
	register unsigned char *ptr = static_cast< unsigned char* >( _Dst );
    #pragma warning( pop )
	while ( _Size-- > 0 )
		*ptr++ = _Val;
	return _Dst;
}
// cause: prj_sysw::TinySynapticsScroll::Legacy::SynapticTouchPad::DoScroll()
// @insp https://codebrowser.dev/glibc/glibc/stdlib/labs.c.html
_Check_return_ 
long // NOLINT(runtime/int)
__cdecl labs(_In_ 
long // NOLINT(runtime/int)
_X) { 
	return ( _X < 0 ) ?-_X :_X;
}
#endif // _DEBUG
