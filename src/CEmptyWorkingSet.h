// src\CEmptyWorkingSet.h - ... useless?
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll {
template<class T = Tray< > >
class CEmptyWorkingSet {
	static const DWORD c_dwClearMemTimerId = 1;
	static const UINT c_uPeriodMemClear_sec = 5;
	const HWND c_hWnd;
	const HANDLE c_hCurProc;
	
 public:
	explicit CEmptyWorkingSet(const T &oTray)
		: c_hWnd( oTray.getHwnd( ) )
		, c_hCurProc( ::GetCurrentProcess( ) ) {
		::SetTimer( c_hWnd, c_dwClearMemTimerId, c_uPeriodMemClear_sec *1000, nullptr );
	}
	void handleWindowMessage(const MSG &stMsg) const {
		if ( WM_TIMER == stMsg.message ) 
			if ( c_dwClearMemTimerId == stMsg.wParam && c_hWnd == stMsg.hwnd )
				::EmptyWorkingSet( c_hCurProc ); // K32EmptyWorkingSet()
	}
	CEmptyWorkingSet &operator = (const CEmptyWorkingSet &) = delete;
};
}} // namespace prj_sysw::TinySynapticsScroll _
