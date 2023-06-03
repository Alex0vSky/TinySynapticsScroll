#include "pch.h"

MOCK_STDCALL_FUNC(ULONGLONG, GetTickCount64 );

namespace testRestoreTouchPadFromSuspend {

class Context : public ::testing::Test {
protected:
	const HWND m_hWnd = (HWND)21;
	static const DWORD c_dwRestoreTouchpadTimerId = 2;
	static const UINT uPeriod_sec = 1;
	MockWrapperTouchPad m_oMockWrapperTouchPad;
	RestoreTouchPadFromSuspend< MockWrapperTouchPad > m_oRestoreTouchPadFromSuspend;
	Context() 
		: m_oRestoreTouchPadFromSuspend( &m_oMockWrapperTouchPad )
	{}
	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetTickCount64 );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( KillTimer );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( SetTimer );
	}
};

typedef Context WM_TIMER_;
NAMESPACE_TEST_F(RestoreTouchPadFromSuspend_HandleWindowMessage, WM_TIMER_, relaunch_and_next_timer) {
	EXPECT_CALL( m_oMockWrapperTouchPad, relaunch( ) );
	EXPECT_MODULE_FUNC_CALL( GetTickCount64 )
		.WillOnce( Return( 0 ) );
	EXPECT_TRUE( m_oRestoreTouchPadFromSuspend.handleWindowMessage( m_hWnd, WM_TIMER, c_dwRestoreTouchpadTimerId, 0 ) );
}
NAMESPACE_TEST_F(RestoreTouchPadFromSuspend_HandleWindowMessage, WM_TIMER_, relaunch_and_stop_timer) {
	EXPECT_CALL( m_oMockWrapperTouchPad, relaunch( ) );
	EXPECT_MODULE_FUNC_CALL( GetTickCount64 )
		.WillOnce( Return( 1 ) );
	EXPECT_MODULE_FUNC_CALL( KillTimer, m_hWnd, c_dwRestoreTouchpadTimerId );
	EXPECT_TRUE( m_oRestoreTouchPadFromSuspend.handleWindowMessage( m_hWnd, WM_TIMER, c_dwRestoreTouchpadTimerId, 0 ) );
}

typedef Context WM_POWERBROADCAST_;
NAMESPACE_TEST_F(RestoreTouchPadFromSuspend_HandleWindowMessage, WM_POWERBROADCAST_, any) {
	EXPECT_MODULE_FUNC_CALL( GetTickCount64 );
	EXPECT_MODULE_FUNC_CALL( SetTimer, m_hWnd, c_dwRestoreTouchpadTimerId, _, nullptr );
	EXPECT_TRUE( m_oRestoreTouchPadFromSuspend.handleWindowMessage( m_hWnd, WM_POWERBROADCAST, MAKEWPARAM( PBT_APMRESUMEAUTOMATIC , 0 ), 0 ) );
}

typedef Context UnknownMessage;
NAMESPACE_TEST_F(RestoreTouchPadFromSuspend_HandleWindowMessage, UnknownMessage, all) {
	EXPECT_FALSE( m_oRestoreTouchPadFromSuspend.handleWindowMessage( m_hWnd, WM_USER, 0, 0 ) );
}

} // namespace testRestoreTouchPadFromSuspend _
