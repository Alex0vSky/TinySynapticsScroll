#include "pch.h"

MOCK_STDCALL_FUNC(VOID, PostQuitMessage, int);

namespace testWndProc {

struct MockLegacySynapticTouchPad : public Legacy::SynapticTouchPad { 
	MOCK_METHOD(bool, run, () );
	MOCK_METHOD(void, stop, () );
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollAccEnabled;
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollLinearEdge;

	struct { 
		DWORD m_dwValue;
		auto operator=(DWORD dwValue) { 
			m_dwValue = dwValue;
			Assign( dwValue );
			return this;
		}
		MOCK_METHOD(void, Assign, (DWORD) );
	} scrollMode;
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollCompatible;
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollSmooth;

	struct { unsigned int Position; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollAcc;
	struct { unsigned int Position; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollSpeed;
};

struct MockRestoreTouchPadFromSuspend_ {
	explicit MockRestoreTouchPadFromSuspend_(WrapperTouchPad<> *poTouchPad)
	{}
	MOCK_METHOD(bool, handleWindowMessage, (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) );
};
typedef MockRestoreTouchPadFromSuspend_ MockRestoreTouchPadFromSuspend;

struct MockTray {
	MOCK_METHOD(bool, handleWindowMessage, (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) );
};
struct MockMenu {
	MOCK_METHOD(bool, handleWindowMessage, (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) );
};

struct WndProcPublicMember_ : public detail_::WndProc_< MockRestoreTouchPadFromSuspend, MockTray, MockMenu > {
	using WndProc_::WndProc_;
	using WndProc_::m_oRestoreTouchPad;
	using WndProc_::m_poTray;
	using WndProc_::m_poMenu;
	void setHwnd(HWND hWnd) {
		m_hWnd = hWnd;
	}
	HWND getHwnd() const {
		return m_hWnd;
	}
};

class WndProcPublicMember : public WndProcPublicMember_, public WndProcPublicMember_::parrent_t {
	using WndProcPublicMember_::WndProcPublicMember_;
	WndProcPublicMember() = delete;
};

class Context : public ::testing::Test {
protected:
	MockWrapperTouchPad m_oMockWrapperTouchPad;
	MockTray m_oMockTray;
	MockMenu m_oMockMenu;
	WndProcPublicMember m_oWndProc;
	HWND m_hWnd = (HWND)5;
	Context() 
		: m_oWndProc( nullptr, &m_oMockTray, &m_oMockMenu )
	{}
	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( SetWindowLongA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetWindowLongA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( DestroyWindow );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( PostQuitMessage );
	}
};

//typedef Context Initial;
NAMESPACE_TEST_F(WndProc_, Context, first) {
	CREATESTRUCTA m_lpCreateStructA{ };
	LONG loWndProc = reinterpret_cast< LONG >( &m_oWndProc );
	EXPECT_MODULE_FUNC_CALL( SetWindowLongA, m_hWnd, GWLP_USERDATA, loWndProc )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_MODULE_FUNC_CALL( GetWindowLongA, m_hWnd, GWLP_USERDATA )
		.WillRepeatedly( Return( loWndProc ) );

	m_lpCreateStructA.lpCreateParams = &m_oWndProc;

	EXPECT_CALL( m_oWndProc.m_oRestoreTouchPad, handleWindowMessage( m_hWnd, WM_NCCREATE, 0, LPARAM( &m_lpCreateStructA ) ) );
	EXPECT_CALL( m_oMockTray, handleWindowMessage( m_hWnd, WM_NCCREATE, 0, LPARAM( &m_lpCreateStructA ) ) );
	EXPECT_CALL( m_oMockMenu, handleWindowMessage( m_hWnd, WM_NCCREATE, 0, LPARAM( &m_lpCreateStructA ) ) );

	WndProcPublicMember::systemCallback( m_hWnd, WM_NCCREATE, 0, LPARAM( &m_lpCreateStructA ) );
	EXPECT_EQ( m_hWnd, m_oWndProc.getHwnd( ) );
}

NAMESPACE_TEST_F(WndProc_, Context, childHandled) {
	EXPECT_MODULE_FUNC_CALL( GetWindowLongA, m_hWnd, GWLP_USERDATA )
		.WillRepeatedly( Return( reinterpret_cast< LONG >( &m_oWndProc ) ) );
	m_oWndProc.setHwnd( m_hWnd );

	EXPECT_CALL( m_oWndProc.m_oRestoreTouchPad, handleWindowMessage( m_hWnd, WM_USER, 0, 0 ) )
		.WillOnce( Return( true ) );
	EXPECT_EQ( 0, WndProcPublicMember::systemCallback( m_hWnd, WM_USER, 0, 0 ) );
}

NAMESPACE_TEST_F(WndProc_, Context, WM_CLOSE_) {
	EXPECT_MODULE_FUNC_CALL( GetWindowLongA, m_hWnd, GWLP_USERDATA )
		.WillRepeatedly( Return( reinterpret_cast< LONG >( &m_oWndProc ) ) );
	m_oWndProc.setHwnd( m_hWnd );

	EXPECT_CALL( m_oWndProc.m_oRestoreTouchPad, handleWindowMessage( _, _, _, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oMockTray, handleWindowMessage( _, _, _, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oMockMenu, handleWindowMessage( _, _, _, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_MODULE_FUNC_CALL( DestroyWindow, m_hWnd )
		.Times( 1 );

	EXPECT_EQ( 0, WndProcPublicMember::systemCallback( m_hWnd, WM_CLOSE, 0, 0 ) );
}

NAMESPACE_TEST_F(WndProc_, Context, WM_DESTROY_) {
	EXPECT_MODULE_FUNC_CALL( GetWindowLongA, m_hWnd, GWLP_USERDATA )
		.WillRepeatedly( Return( reinterpret_cast< LONG >( &m_oWndProc ) ) );
	m_oWndProc.setHwnd( m_hWnd );

	EXPECT_CALL( m_oWndProc.m_oRestoreTouchPad, handleWindowMessage( _, _, _, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oMockTray, handleWindowMessage( _, _, _, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oMockMenu, handleWindowMessage( _, _, _, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_MODULE_FUNC_CALL( PostQuitMessage, 0 )
		.Times( 1 );

	EXPECT_EQ( 0, WndProcPublicMember::systemCallback( m_hWnd, WM_DESTROY, 0, 0 ) );
}
} // namespace testWndProc _
