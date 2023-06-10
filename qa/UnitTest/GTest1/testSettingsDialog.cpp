#include "pch.h"

MOCK_STDCALL_FUNC(HWND, CreateDialogParamA, HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
MOCK_STDCALL_FUNC(BOOL, EndDialog, HWND hDlg, INT_PTR nResult );
MOCK_STDCALL_FUNC(UINT, IsDlgButtonChecked, HWND hDlg, int nIDButton );

namespace Dialog {

template<class T, class T2>
struct MockDialogPopupSettings : public DialogPopupSettings< T, T2 > {
	using DialogPopupSettings< T, T2 >::DialogPopupSettings;

	void setHwnd(HWND hWnd) { 
		ForwardToInstance::Base::m_hWnd = hWnd;
	}

	// member function init_() in base class
	auto getSettingsAutorun() {
		return &m_oSettingsAutorun;
	}
};

class DialogContext : public ::testing::Test {
protected:
	const HWND m_hWndParrent;
	const HWND m_hDialog;
	const HMODULE m_hInst;
	DialogContext() 
		: m_hWndParrent( (HWND)1 )
		, m_hDialog( (HWND)2 )
		, m_hInst( ::GetModuleHandleA( NULL ) )
	{}

	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( CreateDialogParamA );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( DestroyWindow );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( EndDialog );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetDlgItem );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( IsDlgButtonChecked );
	}
};

// tmp
struct MockSettingsScroll : public ScrollPublicMember< MockRegistryScroll, MockWrapperTouchPad > {
	using ScrollPublicMember::ScrollPublicMember;
	MOCK_METHOD(bool, loadToUi, (HWND) );
	MOCK_METHOD(bool, disable, ( ScrollPublicMember::enuToggle ) );
	MOCK_METHOD(bool, enable, ( ScrollPublicMember::enuToggle ) );
	MOCK_METHOD(bool, changeMode, ( ScrollPublicMember::enuMode ) );
	MOCK_METHOD(bool, setSliderValue, ( ScrollPublicMember::enuSlider, WORD ) );

};
struct MockSettingsAutorun : public AutorunPublicMember< MockRegistryAutorun > {
	using AutorunPublicMember::AutorunPublicMember;
	MOCK_METHOD(bool, initAndLoadToUi, (HWND) );
	MOCK_METHOD(bool, removeFromAutorun, () );
	MOCK_METHOD(bool, saveToAutorun, () );
};

typedef DialogContext InitialValues;

NAMESPACE_TEST_F(DialogPopupSettings_, InitialValues, FailCreateDialogWnd) {
	MockWrapperTouchPad oMockWrapperTouchPad;
	MockSettingsScroll oScroll( &oMockWrapperTouchPad );
	MockDialogPopupSettings< MockSettingsScroll, MockSettingsAutorun > oDialog( &oScroll );

	EXPECT_MODULE_FUNC_CALL( CreateDialogParamA, 
				m_hInst, static_cast<LPCTSTR>( MAKEINTRESOURCE( IDD_DIALOGBAR ) )
				, m_hWndParrent
				, decltype( oDialog )::parrent_t::systemCallback
				, (LPARAM)&oDialog
			)
		.Times( 1 )
		.WillOnce( Return( (HWND)0 ) );
	EXPECT_FALSE( oDialog.create( m_hWndParrent ) );
}
NAMESPACE_TEST_F(DialogPopupSettings_, InitialValues, FailInitComponent1) {
	MockWrapperTouchPad oMockWrapperTouchPad;
	MockSettingsScroll oScroll( &oMockWrapperTouchPad );
	MockDialogPopupSettings< MockSettingsScroll, MockSettingsAutorun > oDialog( &oScroll );

	EXPECT_MODULE_FUNC_CALL( CreateDialogParamA, 
				m_hInst, static_cast<LPCTSTR>( MAKEINTRESOURCE( IDD_DIALOGBAR ) )
				, m_hWndParrent
				, decltype( oDialog )::parrent_t::systemCallback
				, (LPARAM)&oDialog
			)
		.Times( 1 )
		.WillOnce( Return( (HWND)m_hDialog ) );
	EXPECT_CALL( oScroll, loadToUi( m_hDialog ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	oDialog.setHwnd( m_hDialog );
	EXPECT_FALSE( oDialog.create( m_hWndParrent ) );
}
NAMESPACE_TEST_F(DialogPopupSettings_, InitialValues, FailInitComponent2) {
	MockWrapperTouchPad oMockWrapperTouchPad;
	MockSettingsScroll oScroll( &oMockWrapperTouchPad );
	MockDialogPopupSettings< MockSettingsScroll, MockSettingsAutorun > oDialog( &oScroll );

	EXPECT_MODULE_FUNC_CALL( CreateDialogParamA, 
				m_hInst, static_cast<LPCTSTR>( MAKEINTRESOURCE( IDD_DIALOGBAR ) )
				, m_hWndParrent
				, decltype( oDialog )::parrent_t::systemCallback
				, (LPARAM)&oDialog
			)
		.Times( 1 )
		.WillOnce( Return( (HWND)m_hDialog ) );
	EXPECT_CALL( oScroll, loadToUi( m_hDialog ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_CALL( *oDialog.getSettingsAutorun( ), initAndLoadToUi( m_hDialog ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	oDialog.setHwnd( m_hDialog );
	EXPECT_FALSE( oDialog.create( m_hWndParrent ) );
}
NAMESPACE_TEST_F(DialogPopupSettings_, InitialValues, FailAccessToRegistry) {

	MockWrapperTouchPad oMockWrapperTouchPad;
	MockSettingsScroll oScroll( &oMockWrapperTouchPad );
	EXPECT_CALL( oScroll.m_oReg, readDword( _, _ ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	EXPECT_CALL( oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( oScroll.readConfigFromRegistry( ) );

	MockDialogPopupSettings< MockSettingsScroll, MockSettingsAutorun > oDialog( &oScroll );
	
	oDialog.setHwnd( m_hDialog );
	EXPECT_MODULE_FUNC_CALL( CreateDialogParamA, 
				m_hInst, static_cast<LPCTSTR>( MAKEINTRESOURCE( IDD_DIALOGBAR ) )
				, m_hWndParrent
				, decltype( oDialog )::parrent_t::systemCallback
				, (LPARAM)&oDialog
			)
		.Times( 1 )
		.WillOnce( Return( m_hDialog ) );

	EXPECT_CALL( oScroll, loadToUi( m_hDialog ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_CALL( *oDialog.getSettingsAutorun( ), initAndLoadToUi( m_hDialog ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	
	EXPECT_EQ( oDialog.getHandle( ), m_hDialog );
	EXPECT_TRUE( oDialog.create( m_hWndParrent ) );
}

class CDialogFunc : public DialogContext {
protected:
	MockWrapperTouchPad m_oMockWrapperTouchPad;
	MockSettingsScroll m_oScroll;
	typedef MockDialogPopupSettings< MockSettingsScroll, MockSettingsAutorun > Dialog_t;
	Dialog_t m_oDialog;
	decltype( Dialog_t::systemCallback ) *m_pfnSystemCallback;
	CDialogFunc() 
		: m_oScroll( &m_oMockWrapperTouchPad )
		, m_oDialog( &m_oScroll )
		, m_pfnSystemCallback( &Dialog_t::systemCallback )
	{
		m_oDialog.setHwnd( m_hDialog );
	}
	void SetUp() override {
		// Override only for return value
		EXPECT_MODULE_FUNC_CALL( GetWindowLongPtrA, m_hDialog, GWLP_USERDATA )
			.WillRepeatedly( Return( (LONG_PTR)&m_oDialog ) );
	}
};

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, uselessMessage) {
	EXPECT_FALSE( m_pfnSystemCallback( m_hDialog, WM_USER, 0, 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Scroll_enable_Acceleration) {
	EXPECT_MODULE_FUNC_CALL( IsDlgButtonChecked, m_hDialog, IDC_CHECK_Acceleration )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_CALL( m_oScroll, enable( MockSettingsScroll::enuToggle::Acceleration ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	auto asd0 = (LONG_PTR)&m_oDialog;
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Acceleration, 0 ), 0 ) );
	VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( IsDlgButtonChecked );
}
NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Scroll_disable_Acceleration) {
	EXPECT_CALL( m_oScroll, disable( MockSettingsScroll::enuToggle::Acceleration ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Acceleration, 0 ), 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Scroll_enable_LinearEdge) {
	EXPECT_MODULE_FUNC_CALL( IsDlgButtonChecked, m_hDialog, IDC_CHECK_Keep_scrolling_on_edges )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_CALL( m_oScroll, enable( MockSettingsScroll::enuToggle::LinearEdge ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Keep_scrolling_on_edges, 0 ), 0 ) );
	VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( IsDlgButtonChecked );
}
NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Scroll_disable_LinearEdge) {
	EXPECT_CALL( m_oScroll, disable( MockSettingsScroll::enuToggle::LinearEdge ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Keep_scrolling_on_edges, 0 ), 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Scroll_changeCompatible) {
	EXPECT_CALL( m_oScroll, changeMode( MockSettingsScroll::enuMode::Compatible ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_RADIO_ScrollMode_Compatible, 0 ), 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Scroll_changeSmooth) {
	EXPECT_CALL( m_oScroll, changeMode( MockSettingsScroll::enuMode::Smooth ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_RADIO_ScrollMode_Smooth, 0 ), 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Autorun_save_succ) {
	EXPECT_MODULE_FUNC_CALL( IsDlgButtonChecked, m_hDialog, IDC_CHECK_Launch_with_Windows )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_CALL( *m_oDialog.getSettingsAutorun( ), saveToAutorun( ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Launch_with_Windows, 0 ), 0 ) );
	VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( IsDlgButtonChecked );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Autorun_remove) {
	EXPECT_CALL( *m_oDialog.getSettingsAutorun( ), removeFromAutorun( ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Launch_with_Windows, 0 ), 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, Autorun_save_fail) {
	EXPECT_MODULE_FUNC_CALL( IsDlgButtonChecked, m_hDialog, IDC_CHECK_Launch_with_Windows )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_CALL( *m_oDialog.getSettingsAutorun( ), saveToAutorun( ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	EXPECT_MODULE_FUNC_CALL( MessageBoxA, _, _, _, _ )
		.Times( ::testing::AnyNumber( ) );
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, MAKEWPARAM( IDC_CHECK_Launch_with_Windows, 0 ), 0 ) );
	VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( IsDlgButtonChecked );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, onDestroyWindow) {
	EXPECT_MODULE_FUNC_CALL( DestroyWindow, m_hDialog )
		.Times( 1 );
	
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_SYSCOMMAND, MAKEWPARAM( SC_CLOSE, 0 ), 0 ) );
}

NAMESPACE_TEST_F(DialogPopupSettings_, CDialogFunc, onEsc) {
	WPARAM xParam = MAKEWPARAM( IDCANCEL, 0 );
	EXPECT_MODULE_FUNC_CALL( EndDialog, m_hDialog, (INT_PTR)xParam )
		.Times( 1 );
	
	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_COMMAND, xParam, 0 ) );
	EXPECT_EQ( m_oDialog.getHandle( ), nullptr );
}

class DialogFuncScroll : public CDialogFunc {
protected:
	WORD m_wRequestKind = SB_ENDSCROLL;
	HWND m_hSlider = (HWND)0x777;
	// Cause return error from ::SendMessageA( hSlider, TBM_GETPOS, 0, 0 );
	WORD m_wPosition = 0;
};
NAMESPACE_TEST_F(DialogPopupSettings_, DialogFuncScroll, scrollSpeed) {

	EXPECT_MODULE_FUNC_CALL( GetDlgItem, m_hDialog, IDC_SLIDER_Speed )
		.WillRepeatedly( Return( m_hSlider ) );
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, m_hDialog, IDC_SLIDER_Acceleration )
		.Times( ::testing::AnyNumber( ) );

	EXPECT_CALL( m_oScroll, setSliderValue( MockSettingsScroll::enuSlider::SpeedValue, m_wPosition ) ).Times( 1 );

	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_HSCROLL, MAKEWPARAM( m_wRequestKind, 0 ), (LPARAM)m_hSlider ) );
}
NAMESPACE_TEST_F(DialogPopupSettings_, DialogFuncScroll, scrollAcceleration) {
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, m_hDialog, IDC_SLIDER_Speed )
		.Times( ::testing::AnyNumber( ) );
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, m_hDialog, IDC_SLIDER_Acceleration )
		.WillRepeatedly( Return( m_hSlider ) );

	EXPECT_CALL( m_oScroll, setSliderValue( MockSettingsScroll::enuSlider::AccelerationValue, m_wPosition ) ).Times( 1 );

	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_HSCROLL, MAKEWPARAM( m_wRequestKind, 0 ), (LPARAM)m_hSlider ) );
}
NAMESPACE_TEST_F(DialogPopupSettings_, DialogFuncScroll, scrollWrongReq) {
	m_wRequestKind = SB_THUMBPOSITION;
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, m_hDialog, _ )
		.Times( 0 );

	EXPECT_TRUE( m_pfnSystemCallback( m_hDialog, WM_HSCROLL, MAKEWPARAM( m_wRequestKind, 0 ), (LPARAM)m_hSlider ) );
}
} // namespace Dialog _
