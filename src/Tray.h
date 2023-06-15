// src\Tray.h - systemtray, create and process window messages
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { 
class WndProc;
template<class T=WndProc, class T2=Menu< > >
class Tray {
	// Must be a valid or LoadIcon() == ERROR_RESOURCE_TYPE_NOT_FOUND;
	const HINSTANCE m_hInst;
	const HICON m_hIcon;
	const UINT c_uWM_TASKBAR_CREATE;
	HWND m_hHiddenWnd;
	static const UINT c_uIdTrayIcon = 1;
	NOTIFYICONDATAA m_stNotifyIconData;

	template <std::size_t N1, std::size_t N2>
	static constexpr void str_chars_(const char (&szSrc)[N1], char (&szDst)[N2]) {
		static_assert( N1 <= N2, "source 'szSrc' string is too long" );
		for ( std::size_t i = 0; i < N1 && i < N2; ++i ) 
			szDst[ i ] = szSrc[ i ];
	}
public:
	explicit Tray(HINSTANCE hInst)
		: m_hInst( hInst )
		, m_hIcon( ::LoadIconA( hInst, static_cast<LPCTSTR>( MAKEINTRESOURCEA( IDI_ICON1 ) ) ) )
		, c_uWM_TASKBAR_CREATE( ::RegisterWindowMessageA( "TaskbarCreated" ) )
		, m_hHiddenWnd( nullptr )
		, m_stNotifyIconData{ }
	{}
	
	bool init(WNDPROC lpfnWndProc, T *poWndProc, const T2 &oMenu) {
		char szClassName[] = "TinySynapticsScrollApplication";
		if ( !m_hIcon ) 
			return false;
		// @insp https://github.com/marek/trayframework/blob/master/trayframework.c
		const INITCOMMONCONTROLSEX stIccex = { sizeof( INITCOMMONCONTROLSEX ), ICC_UPDOWN_CLASS | ICC_LISTVIEW_CLASSES };
		if ( !::InitCommonControlsEx( &stIccex ) ) 
			return false;
		WNDCLASSEXA stWc;
		Tool::Zeroid( stWc );
		stWc.cbSize = sizeof( stWc ); 
		stWc.style = CS_HREDRAW | CS_VREDRAW; 
		stWc.lpfnWndProc = lpfnWndProc;
		stWc.hInstance = m_hInst;
		stWc.hIcon = m_hIcon; 
		stWc.lpszClassName = szClassName;
		stWc.hIconSm = m_hIcon;
		if ( !::RegisterClassExA( &stWc ) )
			return false;

		// Create the hidden window
		m_hHiddenWnd = ::CreateWindowExA( 
				WS_EX_CLIENTEDGE 
				, szClassName, ""
				, WS_OVERLAPPEDWINDOW
				, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
				, NULL, NULL, m_hInst
				, poWndProc
			);
		if ( !m_hHiddenWnd ) 
			return false;

		Tool::Zeroid( m_stNotifyIconData ); m_stNotifyIconData.cbSize = sizeof( m_stNotifyIconData );
		m_stNotifyIconData.hWnd = m_hHiddenWnd;
		m_stNotifyIconData.uID = c_uIdTrayIcon;
		m_stNotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_stNotifyIconData.uCallbackMessage = oMenu.getWindowMessageId( );
		m_stNotifyIconData.hIcon = m_hIcon;
#ifdef _DEBUG
		str_chars_( "TinySynapticsScroll\nSettings or exit [DEBUG]", m_stNotifyIconData.szTip );
#else
		str_chars_( "TinySynapticsScroll\nSettings or exit", m_stNotifyIconData.szTip );
#endif
		// Display tray icon
		if ( !::Shell_NotifyIconA( NIM_ADD, &m_stNotifyIconData ) ) 
			return false;
		return true;
	}
	bool handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM, LPARAM) {
		// Taskbar has been recreated, cause: process explorer.exe has been restarted
		if ( uMsg == c_uWM_TASKBAR_CREATE ) { 
			if ( ::Shell_NotifyIconA( NIM_ADD, &m_stNotifyIconData ) ) 
				return true;
			Tool::ErrorHandler::showMsg( "Systray icon recreation" );
			::DestroyWindow( hWnd );
		}
		// Remove Tray Item
		if ( WM_DESTROY == uMsg ) {
			::Shell_NotifyIconA( NIM_DELETE, &m_stNotifyIconData );
			m_hHiddenWnd = nullptr;
		}
		// yes, false and pass to caller
		return false;
	}
	HWND getHwnd( ) const {
		return m_hHiddenWnd;
	}
	Tray &operator = (const Tray &) = delete;
};
}} // namespace prj_sysw::TinySynapticsScroll _
