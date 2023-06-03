#include "pch.h"

namespace SettingsPersistAutorun {
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::StrEq;

NAMESPACE_TEST(PersistSettings_Autorun, InitialValues_, Empty) {
	AutorunPublicMember< MockRegistryAutorun > oAutorun( IDC_CHECK_Launch_with_Windows );
	EXPECT_CALL( oAutorun.m_oReg, readString( _, _, _ ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	EXPECT_TRUE( oAutorun.initAndLoadToUi( 0 ) );
}

template<class T=::testing::NaggyMock< MockRegistryAutorun > >
class AutorunContext : public ::testing::Test {
protected:
	AutorunPublicMember< T > m_oAutorun;
	size_t m_siCurrentFfnLen;
	wchar_t m_wcsFfnCur[ MAX_PATH + 1 ];
	HWND m_hWndDialog;
	const wchar_t *m_wcsValueName;
	AutorunContext() 
		: m_oAutorun( IDC_CHECK_Launch_with_Windows )
		, m_hWndDialog( 0 )
		, m_wcsValueName( Config::Autorun::getRegValueName( ) )
	{
		HMODULE m_hInst = ::GetModuleHandleA( NULL );
		m_siCurrentFfnLen = (size_t)::GetModuleFileNameW( m_hInst, m_wcsFfnCur, MAX_PATH );
		if ( 0 == m_siCurrentFfnLen || MAX_PATH == m_siCurrentFfnLen )
	        throw std::runtime_error{ "failed to known current module path" }; //GTEST_FATAL_FAILURE_( "" );
	}

	void TearDown() override {
		TRY_RESTORE_MODULE_FUNC( GetModuleFileNameW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( CheckDlgButton );
	}
};

typedef AutorunContext< > InitialValues;
NAMESPACE_TEST_F(PersistSettings_Autorun, InitialValues, EqCurrent) {
	Tool::HeapMem oBufAutorun( ( MAX_PATH + 1 ) * sizeof( wchar_t ) );
	std::memcpy( oBufAutorun.getMemPtr( ), m_wcsFfnCur, sizeof( m_wcsFfnCur ) );
	Tool::HeapMem *p1 = &oBufAutorun;
	size_t *p2 = &m_siCurrentFfnLen;

	EXPECT_CALL( m_oAutorun.m_oReg, readString( m_wcsValueName, _, _ ) )
		.Times( 1 )
		.WillOnce( 
				DoAll(
					SetArgPointee< 1 >( *p1 ),
					SetArgPointee< 2 >( *p2 ),
					Return( true )
				)
			)
		;

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );

	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
}

NAMESPACE_TEST_F(PersistSettings_Autorun, InitialValues, FailGetModuleFileNameW) {

	EXPECT_MODULE_FUNC_CALL( GetModuleFileNameW, _, _, _ )
		.Times( 1 )
		.WillOnce( Return( 0 ) );

	EXPECT_CALL( m_oAutorun.m_oReg, readString( m_wcsValueName, _, _ ) )
		.Times( 0 );

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 0 );

	EXPECT_FALSE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
}

NAMESPACE_TEST_F(PersistSettings_Autorun, InitialValues, NeCurrentByLen) {
	size_t n = 0;
	size_t *p2 = &n;
	EXPECT_CALL( m_oAutorun.m_oReg, readString( m_wcsValueName, _, _ ) )
		.Times( 1 )
		.WillOnce( 
			DoAll(
				SetArgPointee< 2 >( *p2 ),
				Return( true )
			)
		);

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 0 );

	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
}

NAMESPACE_TEST_F(PersistSettings_Autorun, InitialValues, NeCurrentByValue) {
	Tool::HeapMem oBufAutorun( ( MAX_PATH + 1 ) * sizeof( wchar_t ) );
	// Put wrong letter in path
	m_wcsFfnCur[ 0 ] = L'@';
	std::memcpy( oBufAutorun.getMemPtr( ), m_wcsFfnCur, sizeof( m_wcsFfnCur ) );
	Tool::HeapMem *p1 = &oBufAutorun;
	size_t *p2 = &m_siCurrentFfnLen;
	EXPECT_CALL( m_oAutorun.m_oReg, readString( m_wcsValueName, _, _ ) )
		.Times( 1 )
		.WillOnce( 
			DoAll(
				SetArgPointee< 1 >( *p1 ),
				SetArgPointee< 2 >( *p2 ),
				Return( true )
			)
		);

	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 0 );

	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
}

NAMESPACE_TEST_F(PersistSettings_Autorun, InitialValues, FailCheckDlgButton) {
	Tool::HeapMem oBufAutorun( ( MAX_PATH + 1 ) * sizeof( wchar_t ) );
	std::memcpy( oBufAutorun.getMemPtr( ), m_wcsFfnCur, sizeof( m_wcsFfnCur ) );
	Tool::HeapMem *p1 = &oBufAutorun;
	size_t *p2 = &m_siCurrentFfnLen;

	EXPECT_CALL( m_oAutorun.m_oReg, readString( m_wcsValueName, _, _ ) )
		.Times( 1 )
		.WillOnce( 
			DoAll(
				SetArgPointee< 1 >( *p1 ),
				SetArgPointee< 2 >( *p2 ),
				Return( true )
			)
		);
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 1 )
		.WillOnce( Return( FALSE ) );
	EXPECT_FALSE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
}

typedef AutorunContext< ::testing::NiceMock< MockRegistryAutorun > > Save;
NAMESPACE_TEST_F(PersistSettings_Autorun, Save, Succ) {
	EXPECT_CALL( m_oAutorun.m_oReg, createAndSetValueString( m_wcsValueName, StrEq( m_wcsFfnCur ) ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	;
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_UNCHECKED )
		.Times( 0 )
	;
	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
	EXPECT_TRUE( m_oAutorun.saveToAutorun( ) );
}

NAMESPACE_TEST_F(PersistSettings_Autorun, Save, Fail) {
	EXPECT_CALL( m_oAutorun.m_oReg, createAndSetValueString( m_wcsValueName, StrEq( m_wcsFfnCur ) ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	;
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_UNCHECKED )
		.Times( 1 )
	;
	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
	EXPECT_FALSE( m_oAutorun.saveToAutorun( ) );
}

typedef AutorunContext< ::testing::NiceMock< MockRegistryAutorun > > Remove;
NAMESPACE_TEST_F(PersistSettings_Autorun, Remove, Succ) {
	EXPECT_CALL( m_oAutorun.m_oReg, deleteValue( m_wcsValueName ) )
		.Times( 1 )
		.WillOnce( Return( true ) );
	;
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 0 )
	;
	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
	EXPECT_TRUE( m_oAutorun.removeFromAutorun( ) );
}
NAMESPACE_TEST_F(PersistSettings_Autorun, Remove, Fail) {
	EXPECT_CALL( m_oAutorun.m_oReg, deleteValue( m_wcsValueName ) )
		.Times( 1 )
		.WillOnce( Return( false ) );
	;
	EXPECT_MODULE_FUNC_CALL( CheckDlgButton, m_hWndDialog, IDC_CHECK_Launch_with_Windows, BST_CHECKED )
		.Times( 1 )
	;
	EXPECT_TRUE( m_oAutorun.initAndLoadToUi( m_hWndDialog ) );
	EXPECT_FALSE( m_oAutorun.removeFromAutorun( ) );
}

} // namespace SettingsPersistAutorun _
