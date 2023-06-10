// src\Tool\Registry.h - little registry operations
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { namespace Tool { 
class Registry {
	const HKEY m_hRootKey;
	HKEY m_hSubKey;
	const REGSAM m_samDesired;
	Registry(HKEY hRootKey, HKEY hSubKey, REGSAM samDesired) 
		: m_hRootKey( hRootKey ), m_hSubKey( hSubKey ), m_samDesired( samDesired )
	{}
public:
	static const size_t s_hInitialSubKey = -1;
	~Registry() {
		::RegCloseKey( m_hRootKey );
		if ( reinterpret_cast< HKEY >( s_hInitialSubKey ) != m_hSubKey )
			::RegCloseKey( m_hSubKey );
	}
	enum class EnuRootKey {
		// HKEY_CURRENT_USER
		HKCU, 
		// HKEY_LOCAL_MACHINE
		HKLM, 
	};
	enum class EnuDesiredAccessRights {
		// KEY_ALL_ACCESS 
		ALL_ACCESS, 
		// KEY_READ 
		READ, 
	};

	bool readString(const wchar_t *wcsValueName, Tool::HeapMem *poBuf, size_t *psiLen = nullptr) const { 
		DWORD cbData = (DWORD)poBuf ->getSize( );
		LSTATUS lst = ::RegGetValueW( m_hSubKey, nullptr, wcsValueName, RRF_RT_REG_SZ, nullptr, poBuf ->getMemPtr( ), &cbData );
		if ( ERROR_SUCCESS != lst )
			return false;
		if ( psiLen )
			*psiLen = ( ( cbData / sizeof( wcsValueName[ 0 ] ) ) - 1 );
		return true;
	}
	bool readDword(const wchar_t *wcsValueName, DWORD *pdwReadValue) const { 
		DWORD dwType = REG_DWORD, cbData = sizeof( DWORD );
		return ERROR_SUCCESS == ::RegQueryValueExW( m_hSubKey, wcsValueName, nullptr, &dwType, reinterpret_cast<LPBYTE>( pdwReadValue ), &cbData );
	}
	bool createAndSetValueDword(const wchar_t *wcsValueName, const DWORD dwValue) const { 
		return ERROR_SUCCESS == ::RegSetValueExW( m_hSubKey, wcsValueName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof( DWORD ) );
	}
	bool createAndSetValueString(const wchar_t *wcsValueName, const wchar_t *wcsValue) const { 
		size_t cchLength = 0; const wchar_t *wcs = wcsValue; while ( *(wcs++) ) ; --wcs; cchLength = (size_t)( wcs - wcsValue ); // aka strlen
		LSTATUS lst = ::RegSetValueExW( m_hSubKey, wcsValueName, 0, REG_SZ, reinterpret_cast<const BYTE *>( wcsValue ), DWORD( cchLength *sizeof( wchar_t ) ) );
		return ERROR_SUCCESS == lst;
	}
	bool deleteValue(const wchar_t *wcsValueName) const { 
		return ERROR_SUCCESS == ::RegDeleteValueW( m_hSubKey, wcsValueName );
	}

	// RVO
	static Registry openAlways(EnuRootKey enuRootKey, const wchar_t *wcsSubKey, EnuDesiredAccessRights enuRights = EnuDesiredAccessRights::READ) {
		HKEY hRootKey = (HKEY)-1;
		if ( false );
		else if ( EnuRootKey::HKCU == enuRootKey )
			hRootKey = HKEY_CURRENT_USER;
		else if ( EnuRootKey::HKLM == enuRootKey )
			hRootKey = HKEY_LOCAL_MACHINE;
		REGSAM samDesired = 0;
		if ( false );
		else if ( EnuDesiredAccessRights::ALL_ACCESS == enuRights )
			samDesired = KEY_ALL_ACCESS;
		else if ( EnuDesiredAccessRights::READ == enuRights )
			samDesired = KEY_READ;
		HKEY hSubKey = reinterpret_cast< HKEY >( s_hInitialSubKey );
		DWORD ulOptions = REG_OPTION_NON_VOLATILE; // default
		if ( ERROR_SUCCESS != ::RegOpenKeyExW( hRootKey, wcsSubKey, ulOptions, samDesired, &hSubKey ) )
			::RegCreateKeyExW( hRootKey, wcsSubKey, 0, nullptr, ulOptions, samDesired, nullptr, &hSubKey, nullptr );
		return Registry( hRootKey, hSubKey, samDesired );
	}
	Registry(const Registry&& rhs) 
		: m_hRootKey( std::move( rhs.m_hRootKey ) )
		, m_hSubKey( std::move( rhs.m_hSubKey ) )
		, m_samDesired( std::move( rhs.m_samDesired ) )
	{}
	Registry(const Registry&) = delete;

	Registry &operator = (const Registry &) = delete;
};
}}} // namespace prj_sysw::TinySynapticsScroll::Tool _
