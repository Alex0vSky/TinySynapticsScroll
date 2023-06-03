// src\Tool\Zeroid.h - struct or array initialization
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { namespace Tool { 
template<class T> void Zeroid(const T &cr_st) { 
	static_assert( std::is_array_v< T > || std::is_class_v< T >, "only array, struct or class" );
	::RtlSecureZeroMemory( (LPVOID)&cr_st, sizeof( cr_st ) );
}
}}} // namespace prj_sysw::TinySynapticsScroll::Tool _
