// src\CEmptyWorkingSet.h - ... useless?
#pragma once // Copyright 2023 Alex0vSky (https://github.com/Alex0vSky)
namespace prj_sysw { namespace TinySynapticsScroll {
template<class T = Tray< >, class T2 = WrapperTouchPad< > >
class CEmptyWorkingSet {
	static const DWORD c_dwClearMemTimerId = 1;
	static const UINT c_uPeriodMemClear_sec = 5;
	const HWND c_hWnd;
	const HANDLE c_hCurProc;
	T2 *m_poTouchPad;
	
 public:
	explicit CEmptyWorkingSet(const T &oTray, T2 *poTouchPad)
		: c_hWnd( oTray.getHwnd( ) )
		, c_hCurProc( ::GetCurrentProcess( ) )
		, m_poTouchPad( poTouchPad ) {
		::SetTimer( c_hWnd, c_dwClearMemTimerId, c_uPeriodMemClear_sec *1000, nullptr );
	}
	void handleWindowMessage(const MSG &stMsg) const {
		if ( WM_TIMER != stMsg.message ) 
			return;
		if ( c_dwClearMemTimerId == stMsg.wParam && c_hWnd == stMsg.hwnd ) {
			//Tool::ErrorHandler::log( "CEmptyWorkingSet::handleWindowMessage\n" );
			ULONGLONG ullLastTick_milli = 0;
			m_poTouchPad ->getLastPacketTickCount( &ullLastTick_milli );
			if ( false 
				|| ( !ullLastTick_milli )
				|| ( ullLastTick_milli + c_uPeriodMemClear_sec *1000 >= ::GetTickCount64( ) ) 
			) {
				//Tool::ErrorHandler::log( "CEmptyWorkingSet::handleWindowMessage EmptyWorkingSet\n" );
				::EmptyWorkingSet( c_hCurProc ); // K32EmptyWorkingSet()
			}
		}
	}
	CEmptyWorkingSet &operator = (const CEmptyWorkingSet &) = delete;
};
}} // namespace prj_sysw::TinySynapticsScroll _
