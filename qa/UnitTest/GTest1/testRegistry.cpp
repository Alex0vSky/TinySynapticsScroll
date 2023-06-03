#include "pch.h"

MOCK_STDCALL_FUNC(LSTATUS, RegOpenKeyExW, HKEY, LPCWSTR, DWORD, REGSAM, PHKEY); // ->"struct ::mock_module_RegOpenKeyExW"
MOCK_STDCALL_FUNC(LSTATUS, RegCreateKeyExW, HKEY, LPCWSTR, DWORD, LPWSTR, DWORD, REGSAM, CONST LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
MOCK_STDCALL_FUNC(LSTATUS, RegQueryValueExW, HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
MOCK_STDCALL_FUNC(LSTATUS, RegGetValueW, HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData);
MOCK_STDCALL_FUNC(LSTATUS, RegDeleteValueW, HKEY hKey, LPCWSTR lpValueName);
MOCK_STDCALL_FUNC(LSTATUS, RegSetValueExW, HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
MOCK_STDCALL_FUNC(LSTATUS, RegCloseKey, HKEY hKey);

namespace { class Environment0 : public ::testing::Environment {
	void TearDown() override {
		TRY_RESTORE_MODULE_FUNC( RegOpenKeyExW );
		TRY_RESTORE_MODULE_FUNC( RegCreateKeyExW );
		TRY_RESTORE_MODULE_FUNC( RegQueryValueExW );
		TRY_RESTORE_MODULE_FUNC( RegGetValueW );
		TRY_RESTORE_MODULE_FUNC( RegDeleteValueW );
		TRY_RESTORE_MODULE_FUNC( RegSetValueExW );
		TRY_RESTORE_MODULE_FUNC( RegCloseKey );
	}
}; auto g_unused = testing::AddGlobalTestEnvironment( new Environment0 ); }

using namespace prj_sysw::TinySynapticsScroll::Tool;

using testing::Return;
using testing::StrEq;
using testing::Eq;


class RegistryContextHkcu : public ::testing::Test {
protected:
	using Registry = prj_sysw::TinySynapticsScroll::Tool::Registry;
	decltype( testing::Eq(HKEY{}) ) m_oMatcher;
	const wchar_t *m_wcsSubKey;
	const wchar_t *m_wcsValueName;
	const Registry::EnuRootKey m_enuRootKey;
	const HKEY m_hRootKey;
	const REGSAM m_samDesired;
	const DWORD m_ulOptions;
	RegistryContextHkcu() 
		: m_oMatcher( testing::Eq( (HKEY)Registry::s_hInitialSubKey ) )
		, m_wcsSubKey( LR"(foo)" )
		, m_wcsValueName( L"bar" )
		, m_enuRootKey( Registry::EnuRootKey::HKCU )
		, m_hRootKey( HKEY_CURRENT_USER )
		, m_samDesired( KEY_READ )
		, m_ulOptions( REG_OPTION_NON_VOLATILE ) // default
	{}
	void TearDown() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegOpenKeyExW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegCreateKeyExW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegQueryValueExW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegGetValueW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegDeleteValueW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegSetValueExW );
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegCloseKey );
	}
};


namespace Open {
typedef RegistryContextHkcu OpenExist;
NAMESPACE_TEST_F(Tool_Registry, OpenExist, Key) {
	EXPECT_MODULE_FUNC_CALL( RegOpenKeyExW, m_hRootKey, StrEq( m_wcsSubKey ), m_ulOptions, m_samDesired, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_SUCCESS ) );
	EXPECT_MODULE_FUNC_CALL( RegCreateKeyExW, _, _, _, _, _, _, _, _, _ )
		.Times( 0 );
	Registry::openAlways( m_enuRootKey, m_wcsSubKey );
	VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegOpenKeyExW );
	VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( RegCreateKeyExW );
}

// Not process RegCreateKeyExW fail 
typedef RegistryContextHkcu CreateNew;
NAMESPACE_TEST_F(Tool_Registry, CreateNew, Key) {
	EXPECT_MODULE_FUNC_CALL( RegOpenKeyExW, m_hRootKey, StrEq( m_wcsSubKey ), m_ulOptions, m_samDesired, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );
	EXPECT_MODULE_FUNC_CALL( RegCreateKeyExW, _, _, _, _, _, _, _, _, _ )
		.Times( 1 );
	Registry::openAlways( m_enuRootKey, m_wcsSubKey );
}

class RegistryContextReadOpened : public RegistryContextHkcu {
protected:
	decltype( Registry::openAlways(decltype(m_enuRootKey){},decltype(m_wcsSubKey){}) ) m_oRegRead;
	RegistryContextReadOpened() 
		: m_oRegRead( Registry::openAlways( m_enuRootKey, m_wcsSubKey ) )
	{}
};

typedef RegistryContextReadOpened Succ;
NAMESPACED_TEST_F(Succ, Dword, Tool_Registry, ReadExist) {
	EXPECT_MODULE_FUNC_CALL( RegQueryValueExW, m_oMatcher, StrEq( m_wcsValueName ), nullptr, _, _, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_SUCCESS ) );
	DWORD dwReadValue = 0;
	bool bReadDword = m_oRegRead.readDword( m_wcsValueName, &dwReadValue );
	EXPECT_TRUE( bReadDword );
}

NAMESPACED_TEST_F(Succ, String, Tool_Registry, ReadExist) {
	EXPECT_MODULE_FUNC_CALL( RegGetValueW, m_oMatcher, nullptr, StrEq( m_wcsValueName ), RRF_RT_REG_SZ, nullptr, _, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_SUCCESS ) );
	size_t siLen = 0;
	Tool::HeapMem oMem;
	bool bReadString = m_oRegRead.readString( m_wcsValueName, &oMem, &siLen );
	EXPECT_TRUE( bReadString );
}

typedef RegistryContextReadOpened Fail;
NAMESPACED_TEST_F(Fail, Dword, Tool_Registry, ReadExist) {
	EXPECT_MODULE_FUNC_CALL( RegQueryValueExW, m_oMatcher, StrEq( m_wcsValueName ), nullptr, _, _, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );
	DWORD dwReadValue = 0;
	bool bReadDword = m_oRegRead.readDword( m_wcsValueName, &dwReadValue );
	EXPECT_FALSE( bReadDword );
}
NAMESPACED_TEST_F(Fail, String, Tool_Registry, ReadExist) {
	EXPECT_MODULE_FUNC_CALL( RegGetValueW, m_oMatcher, nullptr, StrEq( m_wcsValueName ), RRF_RT_REG_SZ, nullptr, _, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );
	Tool::HeapMem oMem;
	bool bReadString = m_oRegRead.readString( m_wcsValueName, &oMem );
	EXPECT_FALSE( bReadString );
}

typedef RegistryContextReadOpened Delete;
NAMESPACE_TEST_F(Tool_Registry, Delete, Value) {

	EXPECT_MODULE_FUNC_CALL( RegDeleteValueW, m_oMatcher, StrEq( m_wcsValueName ) )
		.Times( 1 )
		.WillOnce( Return( ERROR_SUCCESS ) );

	bool bDeleteSucc = m_oRegRead.deleteValue( m_wcsValueName );
	EXPECT_TRUE( bDeleteSucc );

	EXPECT_MODULE_FUNC_CALL( RegDeleteValueW, m_oMatcher, StrEq( m_wcsValueName ) )
		.Times( 1 )
		.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );

	bool bDeleteFail = m_oRegRead.deleteValue( m_wcsValueName );
	EXPECT_FALSE( bDeleteFail );
}
} // namespace Open _


namespace Create {
class CallExpectBeforeTargetCtor_ : public RegistryContextHkcu {
protected:
	const REGSAM m_samDesired;
	CallExpectBeforeTargetCtor_() 
		: m_samDesired( KEY_ALL_ACCESS )
	{
		EXPECT_MODULE_FUNC_CALL( RegOpenKeyExW, m_hRootKey, StrEq( m_wcsSubKey ), m_ulOptions, m_samDesired, _ )
			.Times( 1 )
			.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );
		EXPECT_MODULE_FUNC_CALL( RegCreateKeyExW, m_hRootKey, m_wcsSubKey, 0, nullptr, m_ulOptions, m_samDesired, nullptr, _, nullptr )
			.Times( 1 );
	}
};
class RegistryContextFullAccessOpened : public CallExpectBeforeTargetCtor_ {
protected:
	Registry m_oRegFullAccess;
	RegistryContextFullAccessOpened() 
		: m_oRegFullAccess( Registry::openAlways( m_enuRootKey, m_wcsSubKey, Registry::EnuDesiredAccessRights::ALL_ACCESS ) )
	{}
};

typedef RegistryContextFullAccessOpened Fail;
NAMESPACED_TEST_F(Fail, Dword, Tool_Registry, SetValue) {
	EXPECT_MODULE_FUNC_CALL( RegSetValueExW, m_oMatcher, m_wcsValueName, 0, REG_DWORD, _, _ )
		.Times( 1 )
		.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );
	bool bCreateAndSet = m_oRegFullAccess.createAndSetValueDword( m_wcsValueName, 0 );
	EXPECT_FALSE( bCreateAndSet );
}

NAMESPACED_TEST_F(Fail, String, Tool_Registry, SetValue) {
	std::wstring strValue = L"baz";
	DWORD dwLen = (DWORD)strValue.length( ) * sizeof( decltype( strValue )::value_type );
	EXPECT_MODULE_FUNC_CALL( RegSetValueExW, m_oMatcher, m_wcsValueName, 0, REG_SZ, (const BYTE*)strValue.c_str( ), dwLen )
		.Times( 1 )
		.WillOnce( Return( ERROR_FILE_NOT_FOUND ) );
	bool bCreateAndSet = m_oRegFullAccess.createAndSetValueString( m_wcsValueName, strValue.c_str( ) );
	EXPECT_FALSE( bCreateAndSet );
}

class RegistryContextWithParam : public ::testing::TestWithParam< std::tuple< Registry::EnuRootKey, Registry::EnuDesiredAccessRights > > {
protected:
	using Registry = prj_sysw::TinySynapticsScroll::Tool::Registry;
	const Registry::EnuRootKey m_enuRootKey;
	const Registry::EnuDesiredAccessRights m_enuAccessRights;
	HKEY m_hRootKey;
	REGSAM m_samDesired;
	RegistryContextWithParam() 
		: m_enuRootKey( std::get< 0 >( GetParam( ) ) )
		, m_enuAccessRights( std::get< 1 >( GetParam( ) ) )
	{
		if ( Registry::EnuRootKey::HKCU == m_enuRootKey )
			m_hRootKey = HKEY_CURRENT_USER;
		else if ( Registry::EnuRootKey::HKLM == m_enuRootKey )
			m_hRootKey = HKEY_LOCAL_MACHINE;
		if ( Registry::EnuDesiredAccessRights::ALL_ACCESS == m_enuAccessRights )
			m_samDesired = KEY_ALL_ACCESS;
		else if ( Registry::EnuDesiredAccessRights::READ == m_enuAccessRights )
			m_samDesired = KEY_READ;
	}
};

TEST_P(RegistryContextWithParam, Tool_Registry_Test1 ) {
	HKEY hSubKey = (HKEY)0x12345;
	HKEY *p4 = &hSubKey;
	EXPECT_MODULE_FUNC_CALL( RegOpenKeyExW, m_hRootKey, _, _, m_samDesired, _ )
		.Times( 1 )
		.WillOnce( 
				::testing::DoAll(
					::testing::SetArgPointee< 4 >( *p4 )
					, Return( ERROR_SUCCESS )
				)
			)
		;
	EXPECT_MODULE_FUNC_CALL( RegCreateKeyExW, _, _, _, _, _, _, _, _, _ )
		.Times( 0 );
	EXPECT_MODULE_FUNC_CALL( RegCloseKey, m_hRootKey )
		.Times( 1 );
	EXPECT_MODULE_FUNC_CALL( RegCloseKey, hSubKey )
		.Times( 1 );
	Registry::openAlways( m_enuRootKey, L"foo", m_enuAccessRights );
}

INSTANTIATE_TEST_SUITE_P(
		Instantiation
		, RegistryContextWithParam
		, ::testing::Combine( 
				::testing::Values( Registry::EnuRootKey::HKCU, Registry::EnuRootKey::HKLM )
				, ::testing::Values( Registry::EnuDesiredAccessRights::READ, Registry::EnuDesiredAccessRights::ALL_ACCESS )
			)
	);

} // namespace Create _
