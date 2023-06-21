// src\PersistSettings\Scroll.h - place current touchPad settings values to ui, store changed values in registry
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { namespace PersistSettings { 
template<class T = Tool::Registry, class T2 = WrapperTouchPad<> >
class Scroll {
	bool readOrCreateNew_(const wchar_t *wcsValueName, DWORD *pdwReadValue, const DWORD dwNewValue) const { 
		if ( m_oReg.readDword( wcsValueName, pdwReadValue ) )
			return true;
		*pdwReadValue = dwNewValue;
		return m_oReg.createAndSetValueDword( wcsValueName, dwNewValue );
	}
	const Config::detail_::RegistryValueName m_stNames;
	T2 *m_poTouchPad;
	// Saved states
	DWORD m_dwAccelerationValue, m_dwAccelerationEnable, m_dwLinearEdgeEnable, m_dwMode, m_dwSpeedValue;
	void passConfigToTouchPad_() { 
		m_poTouchPad ->enableAcceleration( Config::detail_::Switcher::c_dwOn == m_dwAccelerationEnable );
		m_poTouchPad ->setValueAcceleration( m_dwAccelerationValue );
		m_poTouchPad ->enableKeepScrollingOnEdges( Config::detail_::Switcher::c_dwOn == m_dwLinearEdgeEnable );
		if ( Config::Movement::Mode::c_dwSmooth == m_dwMode ) 
			m_poTouchPad ->setScrollModeSmooth( );
		else
			m_poTouchPad ->setScrollModeCompatible( );
		m_poTouchPad ->setValueSpeed( m_dwSpeedValue );
	}
	
 protected:
	const T m_oReg;
	
 public:
	Scroll(const Scroll &) = delete;
	explicit Scroll(T2 *poTouchPad) 
		: m_poTouchPad( poTouchPad )
		, m_dwAccelerationValue(0), m_dwAccelerationEnable(0), m_dwLinearEdgeEnable(0), m_dwMode(0), m_dwSpeedValue(0)
		, m_oReg( 
			T::openAlways( 
				Config::Movement::s_enuRootKey
				, Config::Movement::getRegSubkey( )
				, Tool::Registry::EnuDesiredAccessRights::ALL_ACCESS 
				) 
			)
    {}
	bool readConfigFromRegistry() { 
		using Default = Config::Movement::DefaultSettings;
		if ( !readOrCreateNew_( m_stNames.c_wcsAccelerationValue, &m_dwAccelerationValue, Default::c_dwAccelerationValue ) )
			return false;
		if ( !readOrCreateNew_( 
				m_stNames.c_wcsAccelerationEnable, &m_dwAccelerationEnable, Default::c_dwAccelerationEnable 
			) )
			return false;
		if ( !readOrCreateNew_( m_stNames.c_wcsLinearEdgeEnable, &m_dwLinearEdgeEnable, Default::c_dwLinearEdgeEnable ) )
			return false;
		if ( !readOrCreateNew_( m_stNames.c_wcsMode, &m_dwMode, Default::c_dwMode ) )
			return false;
		if ( !readOrCreateNew_( m_stNames.c_wcsSpeedValue, &m_dwSpeedValue, Default::c_dwSpeedValue ) )
			return false;
		passConfigToTouchPad_( );
		return true;
	}
	// @insp https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-dialog-boxes // @insp https://stackoverflow.com/questions/24065214/how-to-make-checkbox-in-win32 // @insp https://qna.habr.com/q/482567
	bool loadToUi(HWND hWndDialog) const { 
		// By default all Ui controlls are empty/zero
		const BOOL bRedraw = TRUE;
		HWND hSliderAcceleration = ::GetDlgItem( hWndDialog, IDC_SLIDER_Acceleration );
		if ( !hSliderAcceleration ) 
			return false; //LRESULT lrCurPos1 = 
		::SendMessageA( hSliderAcceleration, TBM_SETPOS, bRedraw, (LPARAM)m_dwAccelerationValue );
		if ( m_dwAccelerationEnable ) 
			if ( !::CheckDlgButton( hWndDialog, IDC_CHECK_Acceleration, BST_CHECKED ) )
				return false;
		if ( m_dwLinearEdgeEnable ) 
			if ( !::CheckDlgButton( hWndDialog, IDC_CHECK_Keep_scrolling_on_edges, BST_CHECKED ) )
				return false;
		BOOL bRv;
		switch ( m_dwMode ) {
			default: 
				bRv = ::CheckDlgButton( hWndDialog, IDC_RADIO_ScrollMode_Compatible, BST_CHECKED );
			break;
			case Config::Movement::Mode::c_dwSmooth:
				bRv = ::CheckDlgButton( hWndDialog, IDC_RADIO_ScrollMode_Smooth, BST_CHECKED );
			break;
		}
		if ( !bRv ) 
			return false;
		HWND hSliderSpeed = ::GetDlgItem( hWndDialog, IDC_SLIDER_Speed );
		if ( !hSliderSpeed ) 
			return false; //LRESULT lrCurPos2 = 
		::SendMessageA( hSliderSpeed, TBM_SETPOS, bRedraw, (LPARAM)m_dwSpeedValue );

		return true;
	}

	enum class enuToggle {
		Acceleration, 
		LinearEdge, 
	};
	void enable(enuToggle enu) {
		operation_( enu, Config::detail_::Switcher::c_dwOn );
	}
	void disable(enuToggle enu) {
		operation_( enu, Config::detail_::Switcher::c_dwOff );
	}

	enum class enuMode {
		Smooth, 
		Compatible, 
	};
	void changeMode(enuMode enu) {
		switch ( enu ) {
			case enuMode::Smooth:
				m_oReg.createAndSetValueDword( m_stNames.c_wcsMode, Config::Movement::Mode::c_dwSmooth );
				m_poTouchPad ->setScrollModeSmooth( );
				m_dwMode = Config::Movement::Mode::c_dwSmooth;
			break;
			case enuMode::Compatible:
				m_oReg.createAndSetValueDword( m_stNames.c_wcsMode, Config::Movement::Mode::c_dwCompatible );
				m_poTouchPad ->setScrollModeCompatible( );
				m_dwMode = Config::Movement::Mode::c_dwCompatible;
			break;
		}
	}

	enum class enuSlider {
		AccelerationValue,
		SpeedValue,
	};
	void setSliderValue(enuSlider enu, WORD wValue) {
		switch ( enu ) {
			case enuSlider::AccelerationValue:
		    {
				m_oReg.createAndSetValueDword( m_stNames.c_wcsAccelerationValue, wValue );
				m_poTouchPad ->setValueAcceleration( wValue );
				m_dwAccelerationValue = wValue;
			}
			break;
			case enuSlider::SpeedValue:
				m_oReg.createAndSetValueDword( m_stNames.c_wcsSpeedValue, wValue );
				m_poTouchPad ->setValueSpeed( wValue );
				m_dwSpeedValue = wValue;
			break;
		}
	}

 private:
	void operation_(enuToggle enu, const DWORD dwSwitcher) {
		switch ( enu ) {
			case enuToggle::Acceleration:
				m_oReg.createAndSetValueDword( m_stNames.c_wcsAccelerationEnable, dwSwitcher );
				m_poTouchPad ->enableAcceleration( Config::detail_::Switcher::c_dwOn == dwSwitcher );
				m_dwAccelerationEnable = dwSwitcher;
			break;
			case enuToggle::LinearEdge:
				m_oReg.createAndSetValueDword( m_stNames.c_wcsLinearEdgeEnable, dwSwitcher );
				m_poTouchPad ->enableKeepScrollingOnEdges( Config::detail_::Switcher::c_dwOn == dwSwitcher );
				m_dwLinearEdgeEnable = dwSwitcher;
			break;
		}
	}
	Scroll &operator = (const Scroll &) = delete;
};
}}} // namespace prj_sysw::TinySynapticsScroll::PersistSettings _
