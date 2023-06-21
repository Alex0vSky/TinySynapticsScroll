// src\RestoreTouchPadFromSuspend.h - trickly restore a touchpad when goaway from power suspend mode
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { 
template<class T = WrapperTouchPad< > >
class RestoreTouchPadFromSuspend {
	DWORD m_dwTickFinRestore_milli;
	T *m_poTouchPad;
	static const DWORD c_dwRestoreTouchpadTimerId = 2;
	static const UINT uRestoreDuration_sec = 20;
	static const UINT uPeriod_sec = 1;
	
 public:
	explicit RestoreTouchPadFromSuspend(T *poTouchPad)
		: m_dwTickFinRestore_milli( 0 )
		, m_poTouchPad( poTouchPad )
    {}

	// @insp https://stackoverflow.com/questions/228288/how-can-i-know-when-windows-is-going-into-out-of-sleep-or-hibernate-mode
	bool handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
		if ( WM_POWERBROADCAST == uMsg && PBT_APMRESUMEAUTOMATIC == LOWORD( wParam ) ) {
			m_dwTickFinRestore_milli = (DWORD)::GetTickCount64( ) + uRestoreDuration_sec *1000;
			::SetTimer( hWnd, c_dwRestoreTouchpadTimerId, uPeriod_sec *1000, nullptr );
			return true;
		}
		if ( WM_TIMER == uMsg && c_dwRestoreTouchpadTimerId == wParam ) {
			m_poTouchPad ->relaunch( );
			if ( ::GetTickCount64( ) > m_dwTickFinRestore_milli ) {
				::KillTimer( hWnd, c_dwRestoreTouchpadTimerId );
				m_dwTickFinRestore_milli = 0;
			}
			return true;
		}
		return false;
	}
};
}} // namespace prj_sysw::TinySynapticsScroll _
