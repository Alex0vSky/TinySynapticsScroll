// src\PersistSettings\Autorun.h - place current touchPad autoruns setup to ui, store value in registry
#pragma once // Copyright 2023 Alex0vSky (https://github.com/Alex0vSky)
namespace prj_sysw { namespace TinySynapticsScroll { namespace PersistSettings { 
template<class T = Tool::Registry>
class Autorun {
	HWND m_hWndDialog;
	size_t m_siCurrentFfnLen;
	const int m_nIDButton;
	wchar_t m_wcsFfnCur[ MAX_PATH + 1 ];
	bool check_() const {
		return 0 != ::CheckDlgButton( m_hWndDialog, m_nIDButton, BST_CHECKED );
	}
	
 protected:
	const T m_oReg;
	
 public:
	explicit Autorun(int nIDButton )
		: m_hWndDialog( nullptr )
		, m_siCurrentFfnLen( 0 )
		, m_nIDButton( nIDButton )
		, m_wcsFfnCur{ }
		, m_oReg( 
			T::openAlways( 
				Config::Autorun::s_enuRootKey
				, Config::Autorun::getRegSubkey( )
				, Tool::Registry::EnuDesiredAccessRights::ALL_ACCESS 
				) 
			)
    {}
	bool initAndLoadToUi(HWND hWndDialog) { 
		m_hWndDialog = hWndDialog;
		// GetModuleHandleA not increments the module's reference count
		HMODULE hInst = ::GetModuleHandleA( NULL );
		m_siCurrentFfnLen = static_cast<size_t>( ::GetModuleFileNameW( hInst, m_wcsFfnCur, MAX_PATH ) );
		if ( 0 == m_siCurrentFfnLen || MAX_PATH == m_siCurrentFfnLen )
			return false;
		Tool::HeapMem oBufAutorun;
		size_t siRegFfnLen;
		if ( !m_oReg.readString( Config::Autorun::getRegValueName( ), &oBufAutorun, &siRegFfnLen ) )
			return true;

		// compare strings
		if ( siRegFfnLen != m_siCurrentFfnLen ) 
			return true;
		size_t i;
		const wchar_t *wcsRegFfn = static_cast<wchar_t *>( oBufAutorun.getMemPtr( ) );
		for ( i = 0; i < siRegFfnLen; ++i ) 
			if ( m_wcsFfnCur[ i ] != wcsRegFfn[ i ] )
				break;
		if ( i == siRegFfnLen ) 
			if ( !check_( ) )
				return false;

		return true;
	}
	bool saveToAutorun() const {
		if ( m_oReg.createAndSetValueString( Config::Autorun::getRegValueName( ), m_wcsFfnCur ) ) 
			return true;
		::CheckDlgButton( m_hWndDialog, m_nIDButton, BST_UNCHECKED );
		return false;
	}
	bool removeFromAutorun() const {
		if ( m_oReg.deleteValue( Config::Autorun::getRegValueName( ) ) ) 
			return true;
		check_( );
		return false;
	}

	Autorun &operator = (const Autorun &) = delete;
	Autorun(const Autorun &) = delete;
};
}}} // namespace prj_sysw::TinySynapticsScroll::PersistSettings _
