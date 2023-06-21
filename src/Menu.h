// src\Menu.h - menu, create and process window messages
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll {
template<class T = DialogPopupSettings< >>
class Menu {
	// functon "IsDialogMessage()" uses "WM_USER" and "WM_USER + 1" id's
	static const UINT c_uMessageId = WM_USER + 2;
	T *m_poDialog;

	bool create_(HWND hWnd) const {
		POINT lpClickPoint;
		HMENU hMenu, hSubMenu;
		// get mouse cursor position x and y as lParam has the uMsg itself
		::GetCursorPos( &lpClickPoint );

		//@insp https://learn.microsoft.com/en-us/windows/win32/menurc/using-menus#using-menu-item-bitmaps
		hMenu = ::CreateMenu( );
		hSubMenu = ::CreatePopupMenu( );
		::AppendMenuA( hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "" );
		::AppendMenuA( hSubMenu, MF_STRING, (UINT_PTR)EnuIdPopup::Settings, "Settings" );
		::AppendMenuA( hSubMenu, MF_SEPARATOR, (UINT_PTR)-1, "" );
		::AppendMenuA( hSubMenu, MF_STRING, (UINT_PTR)EnuIdPopup::Exit, "Stop and unload" ); // MFS_DEFAULT = bold

		// Make bold font of last item
		MENUITEMINFOA stMii; 
		Tool::Zeroid( stMii ); stMii.cbSize = sizeof( stMii );
		stMii.fMask = MIIM_STATE;
		::GetMenuItemInfoA( hSubMenu, (UINT)GetMenuItemCount( hSubMenu ) - 1, TRUE, &stMii );
		stMii.fState |= MFS_DEFAULT;
		::SetMenuItemInfoA( hSubMenu, (UINT)GetMenuItemCount( hSubMenu ) - 1, TRUE, &stMii );

		// Display menu
		::SetForegroundWindow( hWnd );
		::TrackPopupMenu( 
				hSubMenu
				, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN
				, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL 
			);
		::SendMessageA( hWnd, WM_NULL, 0, 0 );

		::DestroyMenu( hSubMenu );
		::DestroyMenu( hMenu );
		return true;
	}
	
 protected:
	enum struct EnuIdPopup : WORD {
		Exit,
		Settings,
	};
	
 public:
	Menu(const Menu &) = delete;
	explicit Menu(T *poDialog)
		: m_poDialog( poDialog )
    {}
	bool handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const {
		if ( c_uMessageId == uMsg && WM_RBUTTONDOWN == LOWORD( lParam ) ) {
			if ( create_( hWnd ) )
				return true;
		}
		// Restore dialog by double click
		if ( c_uMessageId == uMsg && WM_LBUTTONDBLCLK == LOWORD( lParam ) ) {
			if ( m_poDialog ->getHandle( ) ) 
				::SetForegroundWindow( hWnd );
			return true;
		}
		// Ctx menu action
		if ( WM_COMMAND == uMsg ) {
			switch( EnuIdPopup( LOWORD( wParam ) ) ) {
				case EnuIdPopup::Settings:
					m_poDialog ->create( hWnd );
					return true;
				break; // @NOCOVERAGE
				case EnuIdPopup::Exit:
					::DestroyWindow( hWnd );
					return true;
				break;
			}
		}
		return false;
	}
	static DWORD getWindowMessageId() {
		return c_uMessageId;
	}
};
}} // namespace prj_sysw::TinySynapticsScroll _
