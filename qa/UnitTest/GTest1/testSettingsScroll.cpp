#include "pch.h"

namespace SettingsPersistScroll {

template<class T,class T2=prj_sysw::TinySynapticsScroll::WrapperTouchPad<> >
struct ScrollAdvPublicMember : public ScrollPublicMember< T,T2 > {
	using ScrollPublicMember< T,T2 >::ScrollPublicMember;
};

class ScrollContext : public ::testing::Test {
protected:
	::testing::NiceMock< MockWrapperTouchPad > m_oMockWrapperTouchPad;
	ScrollAdvPublicMember< MockRegistryScroll, MockWrapperTouchPad > m_oScroll;
	ScrollContext() 
		: m_oScroll( &m_oMockWrapperTouchPad )
	{}
	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( GetDlgItem );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( CheckDlgButton );
	}
};

typedef ScrollContext InitialValues;
NAMESPACE_TEST_F(PersistSettings_Scroll, InitialValues, FailAccessToRegistry) {

	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oScroll.readConfigFromRegistry( ) );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, InitialValues, Empty) {
	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.WillRepeatedly( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, InitialValues, Fake) {
	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, InitialValues, ReadAll) {
	const Config::detail_::RegistryValueName m_stNames;
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsSpeedValue, _ ) )
		.WillOnce( Return( true ) );

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, InitialValues, DefaultSettings) {
	const Config::detail_::RegistryValueName m_stNames;
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsSpeedValue, _ ) )
		.WillOnce( Return( false ) );

	using Default = Config::Movement::DefaultSettings;
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationValue, Default::c_dwAccelerationValue ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationEnable, Default::c_dwAccelerationEnable ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsLinearEdgeEnable, Default::c_dwLinearEdgeEnable ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsMode, Default::c_dwMode ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsSpeedValue, Default::c_dwSpeedValue ) )
		.WillOnce( Return( true ) );

	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, InitialValues, FailAccessToRegistryOrder) {

	const Config::detail_::RegistryValueName m_stNames;
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oScroll.readConfigFromRegistry( ) );

	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oScroll.readConfigFromRegistry( ) );

	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oScroll.readConfigFromRegistry( ) );

	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oScroll.readConfigFromRegistry( ) );

	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationValue, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsSpeedValue, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsSpeedValue, _ ) )
		.WillOnce( Return( false ) );
	EXPECT_FALSE( m_oScroll.readConfigFromRegistry( ) );
}

typedef ScrollContext LoadToUi;
NAMESPACE_TEST_F(PersistSettings_Scroll, LoadToUi, fail1) {
	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );

	HWND hWndDialog = (HWND)1;
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, hWndDialog, IDC_SLIDER_Acceleration )
		.Times( ::testing::AnyNumber( ) )
		.WillRepeatedly( Return( (HWND)NULL ) );

	EXPECT_FALSE( m_oScroll.loadToUi( hWndDialog ) );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, LoadToUi, fail2) {
	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );

	HWND hWndDialog = (HWND)1;
	HWND hSliderAcceleration = (HWND)0x777;
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, hWndDialog, IDC_SLIDER_Acceleration )
		.Times( 1 )
		.WillOnce( Return( hSliderAcceleration ) );

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_RADIO_ScrollMode_Compatible, BST_CHECKED )
		.Times( ::testing::AnyNumber( ) )
		.WillRepeatedly( Return( TRUE ) );

	HWND hSliderSpeed = (HWND)0x333;
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, hWndDialog, IDC_SLIDER_Speed )
		.Times( 1 )
		.WillOnce( Return( hSliderSpeed ) )
		;

	//m_oScroll.m_dwAccelerationEnable = 1;

	m_oScroll.loadToUi( hWndDialog );
}

NAMESPACE_TEST_F(PersistSettings_Scroll, LoadToUi, coverage) {

	HWND hWndDialog = (HWND)1;
	HWND hSliderAcceleration = (HWND)0x777;
	HWND hSliderSpeed = (HWND)0x333;
	const Config::detail_::RegistryValueName m_stNames;
	DWORD dw;
	DWORD *pdw = &dw;

	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	// other
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, hWndDialog, IDC_SLIDER_Acceleration )
		.WillRepeatedly( Return( hSliderAcceleration ) );
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_RADIO_ScrollMode_Compatible, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( TRUE ) );
	EXPECT_MODULE_FUNC_CALL( GetDlgItem, hWndDialog, IDC_SLIDER_Speed )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( hSliderSpeed ) );

	// m_oScroll.m_dwAccelerationEnable = 0x12345;
	dw = (DWORD)0x12345;
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsAccelerationEnable, _ ) )
		.WillOnce( ::testing::DoAll( ::testing::SetArgPointee< 1 >( *pdw ) , Return( true ) ) );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_CHECK_Acceleration, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( TRUE ) );

	m_oScroll.loadToUi( hWndDialog );


	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	// m_oScroll.m_dwLinearEdgeEnable = 0x12345;
	dw = (DWORD)0x12345; 
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsLinearEdgeEnable, _ ) )
		.WillOnce( ::testing::DoAll( ::testing::SetArgPointee< 1 >( *pdw ) , Return( true ) ) );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_CHECK_Keep_scrolling_on_edges, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( TRUE ) );

	m_oScroll.loadToUi( hWndDialog );


	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	// m_oScroll.m_dwMode = Config::Movement::Mode::c_dwSmooth;
	dw = (DWORD)Config::Movement::Mode::c_dwSmooth; 
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( ::testing::DoAll( ::testing::SetArgPointee< 1 >( *pdw ) , Return( true ) ) );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_RADIO_ScrollMode_Smooth, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( TRUE ) );
	m_oScroll.loadToUi( hWndDialog );


	EXPECT_MODULE_FUNC_CALL( GetDlgItem, hWndDialog, IDC_SLIDER_Speed )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( (HWND)NULL ) );
	m_oScroll.loadToUi( hWndDialog );

	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	// m_oScroll.m_dwMode = Config::Movement::Mode::c_dwCompatible;
	dw = (DWORD)Config::Movement::Mode::c_dwCompatible; 
	EXPECT_CALL( m_oScroll.m_oReg, readDword( m_stNames.c_wcsMode, _ ) )
		.WillOnce( ::testing::DoAll( ::testing::SetArgPointee< 1 >( *pdw ) , Return( true ) ) );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_RADIO_ScrollMode_Compatible, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( FALSE ) );
	m_oScroll.loadToUi( hWndDialog );

	// reset config
	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_CHECK_Acceleration, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( FALSE ) );
	m_oScroll.loadToUi( hWndDialog );

	// reset config
	EXPECT_CALL( m_oScroll.m_oReg, readDword( _, _ ) )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( _, _ ) )
		.Times( 0 );
	EXPECT_TRUE( m_oScroll.readConfigFromRegistry( ) );
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_CHECK_Acceleration, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( TRUE ) );
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, hWndDialog, IDC_CHECK_Keep_scrolling_on_edges, BST_CHECKED )
		.Times( ::testing::AtLeast( 1 ) )
		.WillRepeatedly( Return( FALSE ) );
	m_oScroll.loadToUi( hWndDialog );

}

typedef ScrollContext Other;
NAMESPACE_TEST_F(PersistSettings_Scroll, Other, enable_disable) {

	const Config::detail_::RegistryValueName m_stNames;

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationEnable, Config::detail_::Switcher::c_dwOn ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, enableAcceleration( true ) );
	m_oScroll.enable( decltype( m_oScroll )::enuToggle::Acceleration );

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationEnable, Config::detail_::Switcher::c_dwOff ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, enableAcceleration( false ) );
	m_oScroll.disable( decltype( m_oScroll )::enuToggle::Acceleration );

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsLinearEdgeEnable, Config::detail_::Switcher::c_dwOn ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, enableKeepScrollingOnEdges( true ) );
	m_oScroll.enable( decltype( m_oScroll )::enuToggle::LinearEdge );

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsLinearEdgeEnable, Config::detail_::Switcher::c_dwOff ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, enableKeepScrollingOnEdges( false ) );
	m_oScroll.disable( decltype( m_oScroll )::enuToggle::LinearEdge );

}

NAMESPACE_TEST_F(PersistSettings_Scroll, Other, mode) {
	const Config::detail_::RegistryValueName m_stNames;

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsMode, Config::Movement::Mode::c_dwSmooth ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, setScrollModeSmooth( ) );
	m_oScroll.changeMode( decltype( m_oScroll )::enuMode::Smooth );

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsMode, Config::Movement::Mode::c_dwCompatible ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, setScrollModeCompatible( ) );
	m_oScroll.changeMode( decltype( m_oScroll )::enuMode::Compatible );

}

NAMESPACE_TEST_F(PersistSettings_Scroll, Other, slider) {
	const Config::detail_::RegistryValueName m_stNames;
	WORD wValue = 0x123;

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsAccelerationValue, wValue ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, setValueAcceleration( wValue ) );
	m_oScroll.setSliderValue( decltype( m_oScroll )::enuSlider::AccelerationValue, wValue );

	EXPECT_CALL( m_oScroll.m_oReg, createAndSetValueDword( m_stNames.c_wcsSpeedValue, wValue ) )
		.WillRepeatedly( Return( true ) );
	EXPECT_CALL( m_oMockWrapperTouchPad, setValueSpeed( wValue ) );
	m_oScroll.setSliderValue( decltype( m_oScroll )::enuSlider::SpeedValue, wValue );
}

} // namespace SettingsPersistScroll _
