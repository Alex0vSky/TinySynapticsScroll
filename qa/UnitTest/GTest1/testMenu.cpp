#include "pch.h"

MOCK_STDCALL_FUNC(BOOL, SetForegroundWindow, HWND );
MOCK_STDCALL_FUNC(BOOL, GetCursorPos, LPPOINT lpPoint );
MOCK_STDCALL_FUNC(HMENU, CreateMenu );
MOCK_STDCALL_FUNC(HMENU, CreatePopupMenu );
MOCK_STDCALL_FUNC(BOOL, AppendMenuA, HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCSTR lpNewItem);
MOCK_STDCALL_FUNC(BOOL, GetMenuItemInfoA, HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFOA lpmii);
MOCK_STDCALL_FUNC(int, GetMenuItemCount, HMENU hMenu);
MOCK_STDCALL_FUNC(BOOL, SetMenuItemInfoA, HMENU hmenu, UINT item, BOOL fByPositon, LPCMENUITEMINFOA lpmii);
MOCK_STDCALL_FUNC(BOOL, TrackPopupMenu, HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, CONST RECT *prcRect);
MOCK_STDCALL_FUNC(BOOL, DestroyMenu, HMENU hMenu);

namespace testMenu {

struct MockDialogPopupSettings {
	MOCK_METHOD(HWND, getHandle, ( ), (const) );
	MOCK_METHOD(bool, create, (HWND) );
};

struct MenuPublicMember : public Menu< MockDialogPopupSettings > {
	using Menu::Menu;
	using EnuIdPopup = Menu::EnuIdPopup;
};

class Context : public ::testing::Test {
protected:
	static const UINT c_uMessageId = WM_USER + 2;
	const HWND m_hWnd = (HWND)21;
	const HWND m_hDialog = (HWND)22;
	MenuPublicMember m_oMenu;
	MockDialogPopupSettings m_oDialogPopupSettings;
	Context() 
		: m_oMenu( &m_oDialogPopupSettings )
	{}
	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( DestroyWindow );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( SetForegroundWindow );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetCursorPos );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( CreateMenu );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( CreatePopupMenu );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( AppendMenuA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetMenuItemInfoA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetMenuItemCount );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( SetMenuItemInfoA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( TrackPopupMenu );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( DestroyMenu );
	}
};

typedef Context InteractionWithTray;
NAMESPACE_TEST_F(Menu_, InteractionWithTray, byWindowMessageId) {
	EXPECT_EQ( c_uMessageId, m_oMenu.getWindowMessageId( ) );
}

typedef Context FromTray;
NAMESPACE_TEST_F(Menu_HandleWindowMessage, FromTray, create) {
	EXPECT_MODULE_FUNC_CALL( GetCursorPos, _ );
	EXPECT_MODULE_FUNC_CALL( CreateMenu );
	EXPECT_MODULE_FUNC_CALL( CreatePopupMenu );
	EXPECT_MODULE_FUNC_CALL( AppendMenuA, _, _, _, _ )
		.Times( 4 );
	EXPECT_MODULE_FUNC_CALL( GetMenuItemInfoA, _, _, _, _ );
	EXPECT_MODULE_FUNC_CALL( GetMenuItemCount, _ )
		.Times( 2 );
	EXPECT_MODULE_FUNC_CALL( SetMenuItemInfoA, _, _, _, _ );
	EXPECT_MODULE_FUNC_CALL( SetForegroundWindow, _ );
	EXPECT_MODULE_FUNC_CALL( TrackPopupMenu, _, _, _, _, _, _, _ );
	EXPECT_MODULE_FUNC_CALL( DestroyMenu, _ )
		.Times( 2 );
	EXPECT_TRUE( m_oMenu.handleWindowMessage( m_hWnd, c_uMessageId, 0, MAKELPARAM( WM_RBUTTONDOWN, 0 ) ) );
}

NAMESPACE_TEST_F(Menu_HandleWindowMessage, FromTray, restore) {
	EXPECT_CALL( m_oDialogPopupSettings, getHandle( ) )
		.WillOnce( Return( m_hDialog ) );
	EXPECT_MODULE_FUNC_CALL( SetForegroundWindow, m_hWnd )
		.Times( 1 );
	EXPECT_TRUE( m_oMenu.handleWindowMessage( m_hWnd, c_uMessageId, 0, MAKELPARAM( WM_LBUTTONDBLCLK, 0 ) ) );
}

typedef Context FromContextMenu;
NAMESPACE_TEST_F(Menu_HandleWindowMessage, FromContextMenu, itemExit) {
	EXPECT_MODULE_FUNC_CALL( DestroyWindow, m_hWnd )
		.Times( 1 );
	EXPECT_TRUE( m_oMenu.handleWindowMessage( m_hWnd, WM_COMMAND, MAKEWPARAM( decltype( m_oMenu )::EnuIdPopup::Exit, 0 ), 0 ) );
}

NAMESPACE_TEST_F(Menu_HandleWindowMessage, FromContextMenu, itemSettings) {
	EXPECT_CALL( m_oDialogPopupSettings, create( m_hWnd ) );
	EXPECT_TRUE( m_oMenu.handleWindowMessage( m_hWnd, WM_COMMAND, MAKEWPARAM( decltype( m_oMenu )::EnuIdPopup::Settings, 0 ), 0 ) );
}

typedef Context UnknownMessage;
NAMESPACE_TEST_F(Menu_HandleWindowMessage, UnknownMessage, all) {
	EXPECT_FALSE( m_oMenu.handleWindowMessage( m_hWnd, WM_USER, 0, 0 ) );
}

} // namespace testMenu _
