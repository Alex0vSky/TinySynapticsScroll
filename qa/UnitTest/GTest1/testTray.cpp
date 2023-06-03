#include "pch.h"

MOCK_STDCALL_FUNC(HWND, CreateWindowExA, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
MOCK_STDCALL_FUNC(HICON, LoadIconA, HINSTANCE hInstance, LPCSTR lpIconName);
MOCK_STDCALL_FUNC(UINT, RegisterWindowMessageA, LPCSTR lpString);
MOCK_STDCALL_FUNC(BOOL, InitCommonControlsEx, const INITCOMMONCONTROLSEX *picce);
MOCK_STDCALL_FUNC(ATOM, RegisterClassExA, CONST WNDCLASSEXA *);
MOCK_STDCALL_FUNC(BOOL, Shell_NotifyIconA, DWORD dwMessage, _In_ PNOTIFYICONDATAW lpData);

namespace testTray {

struct MockMenu {
	UINT m_uCallbackMessage = 11;
	MOCK_METHOD(UINT, getWindowMessageId, ( ), (const) );
};

template<HICON VHICON>
class ContextBeforeCtor_ : public ::testing::Test {
protected:
	const HINSTANCE m_hInst = (HINSTANCE)6;
	HICON m_hIcon = (HICON)VHICON;
	const UINT c_uWM_TASKBAR_CREATE = 8;
	ContextBeforeCtor_() {
		EXPECT_MODULE_FUNC_CALL( LoadIconA, m_hInst, _ )
			.WillOnce( Return( m_hIcon ) );
		EXPECT_MODULE_FUNC_CALL( RegisterWindowMessageA, _ )
			.WillOnce( Return( c_uWM_TASKBAR_CREATE ) );
	}
};

template<HICON VHICON=(HICON)7>
class Context_ : public ContextBeforeCtor_< VHICON > {
protected:
	const HWND m_hHiddenWnd = (HWND)9;
	WNDPROC m_lpfnWndProc = nullptr;
	WndProc *m_poWndProc = nullptr;
	MockMenu m_oMenu;
	Tray< WndProc, MockMenu > m_oTray;
	Context_() 
		: m_oTray( m_hInst )
	{}
	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( CreateWindowExA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( LoadIconA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegisterWindowMessageA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( InitCommonControlsEx );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegisterClassExA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( Shell_NotifyIconA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( DestroyWindow );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( MessageBoxA );
	}
};
typedef Context_< > Context;

typedef Context Init;
NAMESPACE_TEST_F(Tray_, Init, succ) {
	EXPECT_MODULE_FUNC_CALL( InitCommonControlsEx, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( RegisterClassExA, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( CreateWindowExA, WS_EX_CLIENTEDGE, _, _, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hInst, m_poWndProc )
		.WillOnce( Return( m_hHiddenWnd ) );
	EXPECT_CALL( m_oMenu, getWindowMessageId( ) )
		.WillOnce( Return( m_oMenu.m_uCallbackMessage ) );
	EXPECT_MODULE_FUNC_CALL( Shell_NotifyIconA, NIM_ADD, _ )
		.WillOnce( Return( true ) );

	EXPECT_TRUE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );
}

typedef Context_< (HICON)NULL > Init2;
NAMESPACE_TEST_F(Tray_, Init2, failLoadIcon) {
	EXPECT_FALSE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );
}

NAMESPACE_TEST_F(Tray_, Init, fails) {
	EXPECT_MODULE_FUNC_CALL( InitCommonControlsEx, _ )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );

	EXPECT_MODULE_FUNC_CALL( InitCommonControlsEx, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( RegisterClassExA, _ )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );

	EXPECT_MODULE_FUNC_CALL( InitCommonControlsEx, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( RegisterClassExA, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( CreateWindowExA, _, _, _, _, _, _, _, _, _, _, _, _ )
		.WillOnce( Return( (HWND)NULL ) );
	EXPECT_FALSE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );

	EXPECT_MODULE_FUNC_CALL( InitCommonControlsEx, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( RegisterClassExA, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( CreateWindowExA, _, _, _, _, _, _, _, _, _, _, _, _ )
		.WillOnce( Return( m_hHiddenWnd ) );
	EXPECT_CALL( m_oMenu, getWindowMessageId( ) )
		.WillOnce( Return( m_oMenu.m_uCallbackMessage ) );
	EXPECT_MODULE_FUNC_CALL( Shell_NotifyIconA, NIM_ADD, _ )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );
}

NAMESPACE_TEST_F(Tray_, Context, getHWnd) {
	EXPECT_MODULE_FUNC_CALL( InitCommonControlsEx, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( RegisterClassExA, _ )
		.WillOnce( Return( true ) );
	EXPECT_MODULE_FUNC_CALL( CreateWindowExA, _, _, _, _, _, _, _, _, _, _, _, _ )
		.WillOnce( Return( m_hHiddenWnd ) );
	EXPECT_CALL( m_oMenu, getWindowMessageId( ) );
	EXPECT_MODULE_FUNC_CALL( Shell_NotifyIconA, NIM_ADD, _ )
		.WillOnce( Return( true ) );

	EXPECT_TRUE( m_oTray.init( m_lpfnWndProc, m_poWndProc, m_oMenu ) );
	EXPECT_EQ( m_hHiddenWnd, m_oTray.getHwnd( ) );
}

typedef Context Handle_WM_TASKBAR_CREATE;
NAMESPACE_TEST_F(Tray_, Handle_WM_TASKBAR_CREATE, succ) {
	EXPECT_MODULE_FUNC_CALL( Shell_NotifyIconA, NIM_ADD, _ )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_oTray.handleWindowMessage( m_hHiddenWnd, c_uWM_TASKBAR_CREATE, 0, 0 ) );
}
NAMESPACE_TEST_F(Tray_, Handle_WM_TASKBAR_CREATE, fail) {
	EXPECT_MODULE_FUNC_CALL( Shell_NotifyIconA, NIM_ADD, _ )
		.WillOnce( Return( false ) );
	EXPECT_MODULE_FUNC_CALL( MessageBoxA, _, _, _, _ )
		.Times( ::testing::AnyNumber( ) );
	EXPECT_MODULE_FUNC_CALL( DestroyWindow, m_hHiddenWnd )
		.Times( 1 );
	EXPECT_FALSE( m_oTray.handleWindowMessage( m_hHiddenWnd, c_uWM_TASKBAR_CREATE, 0, 0 ) );
}

typedef Context Handle_WM_DESTROY;
NAMESPACE_TEST_F(Tray_, Handle_WM_DESTROY, all) {
	EXPECT_MODULE_FUNC_CALL( Shell_NotifyIconA, NIM_DELETE, _ )
		.Times( 1 );
	EXPECT_FALSE( m_oTray.handleWindowMessage( m_hHiddenWnd, WM_DESTROY, 0, 0 ) );
	EXPECT_EQ( nullptr, m_oTray.getHwnd( ) );
}

} // namespace testTray _
