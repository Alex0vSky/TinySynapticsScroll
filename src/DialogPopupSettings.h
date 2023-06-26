// src\DialogPopupSettings.h - create popup dialog from menu of tray icon
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { 
namespace detail_ { 
template<class T = PersistSettings::Scroll<>, class T2 = PersistSettings::Autorun<>>
class DialogPopupSettings_ : public ForwardToInstance::Base {
	INT_PTR dialogFunc_(UINT uMsg, WPARAM wParam, LPARAM lParam) { 
		switch ( uMsg )  { 
			default:
				return FALSE;
			case WM_SYSCOMMAND:
				// Close via click SystemMenu on TitleBar
				if ( SC_CLOSE == ( wParam & 0xfff0 ) ) { 
					::DestroyWindow( m_hWnd );
					m_hWnd = nullptr;
				}
			break;
			case WM_COMMAND: 
				switch ( LOWORD( wParam ) ) { 
					// And too keyboard [ESC] pressed
					case IDCANCEL:
						::EndDialog( m_hWnd, (INT_PTR)wParam );
						m_hWnd = nullptr;
					break;
					case IDC_CHECK_Acceleration:
						if ( ::IsDlgButtonChecked( m_hWnd, IDC_CHECK_Acceleration ) ) 
							m_poSettingsScroll ->enable( T::enuToggle::Acceleration );
						else 
							m_poSettingsScroll ->disable( T::enuToggle::Acceleration );
					break;
					case IDC_CHECK_Keep_scrolling_on_edges:
						if ( ::IsDlgButtonChecked( m_hWnd, IDC_CHECK_Keep_scrolling_on_edges ) ) 
							m_poSettingsScroll ->enable( T::enuToggle::LinearEdge );
						else
							m_poSettingsScroll ->disable( T::enuToggle::LinearEdge );
					break;
					case IDC_RADIO_ScrollMode_Compatible:
						m_poSettingsScroll ->changeMode( T::enuMode::Compatible );
					break;
					case IDC_RADIO_ScrollMode_Smooth:
						m_poSettingsScroll ->changeMode( T::enuMode::Smooth );
					break;
					case IDC_CHECK_Launch_with_Windows:
						if ( ::IsDlgButtonChecked( m_hWnd, IDC_CHECK_Launch_with_Windows ) ) {
							// be aware of
							if ( !m_oSettingsAutorun.saveToAutorun( ) )
								Tool::ErrorHandler::showMsg( "Save app autostart" );
						} else {
							m_oSettingsAutorun.removeFromAutorun( );
						}
					break;
				} 
			break;
			case WM_HSCROLL: 
			    {
					HWND hSlider = reinterpret_cast<HWND>( lParam );
					WORD wRequestKind = LOWORD( wParam );
					if ( SB_ENDSCROLL != wRequestKind || !hSlider ) 
						break;
					WORD wPosition = (WORD)::SendMessageA( hSlider, TBM_GETPOS, 0, 0 );
					if ( false ) {}
					else if ( hSlider == ::GetDlgItem( m_hWnd, IDC_SLIDER_Speed ) ) 
						m_poSettingsScroll ->setSliderValue( T::enuSlider::SpeedValue, wPosition );
					else if ( hSlider == ::GetDlgItem( m_hWnd, IDC_SLIDER_Acceleration ) ) 
						m_poSettingsScroll ->setSliderValue( T::enuSlider::AccelerationValue, wPosition );
				}
			break;
		}
		return TRUE;
	}
	
 protected:
	T *m_poSettingsScroll;
	T2 m_oSettingsAutorun;
	bool init_() {
		return m_poSettingsScroll ->loadToUi( m_hWnd ) && m_oSettingsAutorun.initAndLoadToUi( m_hWnd );
	}
	
 public:
	explicit DialogPopupSettings_(T *poSettingsScroll) 
		: m_poSettingsScroll( poSettingsScroll )
		, m_oSettingsAutorun( IDC_CHECK_Launch_with_Windows )
    {}

	HWND getHandle() const {
		return m_hWnd;
	}
	using parrent_t = ForwardToInstance::OfDialog< 
			decltype( &DialogPopupSettings_::dialogFunc_ )
			, &DialogPopupSettings_::dialogFunc_ 
		>;
};
} // namespace detail_ _

template<class T = PersistSettings::Scroll<>, class T2 = PersistSettings::Autorun<>>
class DialogPopupSettings 
	: public detail_::DialogPopupSettings_< T, T2 >
	, public detail_::DialogPopupSettings_< T, T2 >::parrent_t {
	using detail_::DialogPopupSettings_< T, T2 >::DialogPopupSettings_;
 public:
	// @insp https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-dialog-boxes
	bool create(HWND hWndParrent) { 
#pragma warning( push )
#pragma warning( disable: 5039 )
		HWND hWnd = ::CreateDialogParamA( 
				(HINSTANCE)::GetModuleHandleA( NULL )
				, static_cast<LPCTSTR>( MAKEINTRESOURCE( IDD_DIALOGBAR ) )
				, hWndParrent
				, detail_::DialogPopupSettings_< T, T2 >::parrent_t::systemCallback
				, (LPARAM)this
			);
#pragma warning( pop )
		if ( !hWnd )
			return false;
		if ( !detail_::DialogPopupSettings_< T, T2 >::init_( ) )
			return false;
		::ShowWindow( hWnd, SW_SHOW );
		return true;
	}
	DialogPopupSettings() = delete;
};

}} // namespace prj_sysw::TinySynapticsScroll _
