// src\WrapperTouchPad.h - wrap of VCL(Borland C++) legacy software
#pragma once // Copyright 2023 Alex0vSky (https://github.com/Alex0vSky)
namespace prj_sysw { namespace TinySynapticsScroll { 
template<class T = Legacy::SynapticTouchPad>
class WrapperTouchPad {
 protected:
	T m_oSynapticTouchPad;

 public:
	WrapperTouchPad() = default;
	// hWnd is NULL. For normal working SynCOM.dll via ISynDevice::SetSynchronousNotification (WM_APP == stMsg.message)
	static HWND getHwndForMessagesDispatch(HWND) {
		return nullptr;
	}
	bool start() {
		return m_oSynapticTouchPad.run( );
	}
	bool relaunch() {
		m_oSynapticTouchPad.stop( );
		return m_oSynapticTouchPad.run( );
	}
	void enableAcceleration(bool bEnable) {
		m_oSynapticTouchPad.scrollAccEnabled ->Checked = bEnable;
	}
	void enableKeepScrollingOnEdges(bool bEnable) {
		m_oSynapticTouchPad.scrollLinearEdge ->Checked = bEnable;
	}
	void setScrollModeSmooth() {
		m_oSynapticTouchPad.scrollMode = Config::Movement::Mode::c_dwSmooth;
		m_oSynapticTouchPad.scrollCompatible ->Checked = false;
		m_oSynapticTouchPad.scrollSmooth ->Checked = true;
	}
	void setScrollModeCompatible() {
		m_oSynapticTouchPad.scrollMode = Config::Movement::Mode::c_dwCompatible;
		m_oSynapticTouchPad.scrollSmooth ->Checked = false;
		m_oSynapticTouchPad.scrollCompatible->Checked = true;
	}
	void setValueAcceleration(unsigned int uValue) {
		m_oSynapticTouchPad.scrollAcc ->Position = static_cast<int>( uValue );
	}
	void setValueSpeed(unsigned int uValue) {
		m_oSynapticTouchPad.scrollSpeed ->Position = static_cast<int>( uValue );
	}
	WrapperTouchPad(const WrapperTouchPad &) = delete;
	WrapperTouchPad &operator = (const WrapperTouchPad &) = delete;
};
}} // namespace prj_sysw::TinySynapticsScroll _
