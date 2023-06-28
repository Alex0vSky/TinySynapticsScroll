// src\Legacy\SynapticTouchPad.h - activate Synaptic touchPad interaction
// @from https://github.com/awahlig/two-finger-scroll
// NOLINT(legal/copyright)
#pragma once
#ifdef __clang__
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wlanguage-extension-token"
#	pragma clang diagnostic ignored "-Wnon-virtual-dtor"
//#	pragma clang diagnostic ignored "-XXX"
#endif // __clang__
namespace prj_sysw { namespace TinySynapticsScroll { namespace Legacy { 
class SynapticTouchPad : public _ISynDeviceEvents {
	// Names without *Ctrl is usable without create Ole/COM server
	ISynAPI *m_ifcApiTouchPad;
	ISynDevice *m_ifcDevTouchPad;
	ISynPacket *m_ifcPacketTouchPad;

	// Mock VCL(Borland C++) part1
	struct TTrackBar { int Position; int Max; int Min; };
	struct TComboBox { int ItemIndex; };
	struct TCheckBox { bool Checked; };
	struct TRadioButton { bool Checked; };
	long scrollTouchTime;
	POINT scrollTouchPos;
	bool IsPadAcquired;
	TCheckBox scrollLinearEdge_;
	TRadioButton scrollLinear_;
	TRadioButton *scrollLinear;
	TTrackBar scrollSpeed_;
	TCheckBox scrollAccEnabled_;
	TTrackBar scrollAcc_;
	long scrollBuffer;
	bool scrollNotEdge;
	long scrollLastXDelta, scrollLastYDelta;
	TRadioButton scrollCompatible_;
	TRadioButton scrollSmooth_;

	void __fastcall AcquirePad(bool acquire)
	{
		if (acquire && !IsPadAcquired) {
			m_ifcDevTouchPad ->Acquire(0);
		}
		else if (!acquire && IsPadAcquired) {
			m_ifcDevTouchPad ->Unacquire( );
		}
		IsPadAcquired = acquire;
	}
	int __fastcall GetScrollMode()
	{
		int mode = 1;
		// @removed
		return mode;
	}
	bool __fastcall DoScroll(long dx, long dy)
	{
		UNREFERENCED_PARAMETER( dx );

		long d;

		if (abs(dy) > 800)
			return false;

		// scrollSpeed
		dy = dy * scrollSpeed->Position / 100;

		// scrollAcc
		if (scrollAccEnabled->Checked) {
			// @todo: (prj_sysw::TinySynapticsScroll) division by zero!
			d = dy * dy / (scrollAcc->Max - scrollAcc->Position + scrollAcc->Min);
			if (d < 4)
				d = 4;
			if (dy < 0)
				d = -d;
		}
		else d = dy;

		if (scrollMode == 0) {
			// compatibility mode
			scrollBuffer += d;
			d = scrollBuffer - scrollBuffer % WHEEL_DELTA;
		}

		if (d != 0) {
			INPUT i;

			::SecureZeroMemory( &i, sizeof(INPUT) );
			i.type = INPUT_MOUSE;
			i.mi.dwFlags = MOUSEEVENTF_WHEEL;
			i.mi.mouseData = (DWORD)d;
			::SendInput(1, &i, sizeof(INPUT));

			if (scrollMode == 0) // compatibility mode
				scrollBuffer -= d;
		}

		return true;
	}
	// Called from "MainThread" via "::DispatchMessageA();"
	HRESULT STDMETHODCALLTYPE OnSynDevicePacket(long) override {
		// get the pointing data packet
		while ( S_OK == ( m_ifcDevTouchPad ->LoadPacket( m_ifcPacketTouchPad ) ) ) {
			long nof, fstate, xd, yd;
			// extract relevant data
			m_ifcPacketTouchPad ->GetProperty( SP_ExtraFingerState, &nof);
			nof &= 3;
			m_ifcPacketTouchPad ->GetProperty( SP_FingerState, &fstate);
			m_ifcPacketTouchPad ->GetProperty( SP_XDelta, &xd);
			m_ifcPacketTouchPad ->GetProperty( SP_YDelta, &yd);

			// handle scrolling
			if (fstate & SF_FingerPresent) {
				if (scrollTouchTime == 0) {
					::GetCursorPos( &scrollTouchPos );
					m_ifcPacketTouchPad ->GetProperty( SP_TimeStamp,&scrollTouchTime );
				}
				if (nof == 2) {
					long y, ylo, yhi;
					m_ifcPacketTouchPad ->GetProperty(SP_Y, &y);
					m_ifcDevTouchPad ->GetProperty(SP_YLoBorder, &ylo);
					m_ifcDevTouchPad ->GetProperty(SP_YHiBorder, &yhi);
					if (IsPadAcquired && scrollLinearEdge ->Checked) {
						if (ylo <= y && y <= yhi) {
							scrollNotEdge = true;
						}
						else if (scrollNotEdge && ((y < ylo && scrollLastYDelta < 0) ||
								(y > yhi && scrollLastYDelta > 0))) {
							DoScroll(scrollLastXDelta, scrollLastYDelta);
							return 0;
						}
					}
					if (fstate & SF_FingerMotion) {
						if (!IsPadAcquired) {
							AcquirePad(true);
							long tstamp;
							m_ifcPacketTouchPad->GetProperty(SP_TimeStamp, &tstamp);
							if (tstamp - scrollTouchTime < 1000) {
								SetCursorPos(scrollTouchPos.x,
									scrollTouchPos.y);
							}
							if (scrollCompatible->Checked) {
								scrollMode = 0;
							}
							else if (scrollSmooth->Checked) {
								scrollMode = 1;
							}
							else {
								scrollMode = GetScrollMode();
								if ((GetKeyState(VK_SHIFT) & 0x8000) &&
										(GetKeyState(VK_CONTROL) & 0x8000) &&
										(GetKeyState(VK_MENU) & 0x8000)) {
									// toggle scroll mode
									if (scrollMode == 1) scrollMode = 0;
									else scrollMode = 1;
									//SetScrollMode(scrollMode);
								}

							}
						}
						if ( IsPadAcquired ) {
							DoScroll(xd, yd);
							scrollLastXDelta = xd;
							scrollLastYDelta = yd;
						}
					}
				}
				else {
					scrollLastXDelta = scrollLastYDelta = 0;
					AcquirePad( false );
					scrollBuffer = 0;
					scrollNotEdge = false;
				}
			}
			else {
				scrollTouchTime = 0;
				scrollLastXDelta = scrollLastYDelta = 0;
				AcquirePad( false );
				scrollBuffer = 0;
				scrollNotEdge = false;
			}
		}
		return S_OK;
	}

public:
	SynapticTouchPad(const SynapticTouchPad &) = delete;
	SynapticTouchPad() 
		: m_ifcApiTouchPad( nullptr )
		, m_ifcDevTouchPad( nullptr )
		, m_ifcPacketTouchPad( nullptr )
		, scrollTouchPos{ }
		, scrollLinearEdge_{ }
		, scrollLinear_{ }
		, scrollSpeed_{ }
		, scrollAccEnabled_{ }
		, scrollAcc_{ }
		, scrollCompatible_{ }
		, scrollSmooth_{ }
	{
		scrollTouchTime = 0;
				
		IsPadAcquired = false;
		scrollLinearEdge = &scrollLinearEdge_;
		scrollLinear = &scrollLinear_;
		scrollSpeed = &scrollSpeed_;
		scrollAccEnabled = &scrollAccEnabled_;
		scrollAcc = &scrollAcc_;
		scrollCompatible = &scrollCompatible_;
		scrollSmooth = &scrollSmooth_;
		scrollMode = 0;
		scrollBuffer = 0;
		scrollNotEdge = false;
		scrollLastXDelta = scrollLastYDelta = 0;
		scrollLinear ->Checked = true;
		scrollLinearEdge ->Checked = true;
		scrollSpeed ->Position = 100;
		scrollAcc ->Position = 75;
		scrollCompatible ->Checked = true;
		// @from Vcl \TwoFingerScroll_resource\FORM1.dfm
		scrollAcc ->Min = 30;
		scrollAcc ->Max = 120;
		scrollSpeed ->Min = 10;
		scrollSpeed ->Max = 200;
	}
#pragma warning( push )
#pragma warning( disable: 4265 )
	// not virtual, or implement "delete" function // @insp https://devblogs.microsoft.com/oldnewthing/20200619-00/?p=103877
	~SynapticTouchPad() {
		stop( );
	}
#pragma warning( pop )

	bool run() {
		HRESULT hRv; 
		// STA apartment
		if ( FAILED( hRv = ::CoInitializeEx( 0, 0 ) ) ) 
			return false;
			// Com not initialized
		long devHandle = -1;

		if ( FAILED( hRv = ::CoCreateInstance( _uuidof( SynAPI ), 0, CLSCTX_INPROC_SERVER, _uuidof( ISynAPI ), (void **)&m_ifcApiTouchPad ) ) )
			return false;
			// COM API not found, this happens (I guess) if we start before the Synaptics driver is up and running.
		if ( FAILED( hRv = m_ifcApiTouchPad ->Initialize( ) ) ) 
			return false;
			// API was not initialized

		if ( FAILED( hRv = m_ifcApiTouchPad ->FindDevice( SE_ConnectionAny, SE_DeviceTouchPad, &devHandle ) ) ) 
			return false;
			// Error Synaptics TouchPad device find
		if ( devHandle < 0 ) 
			return false;
			// No Synaptics TouchPad device found

		if ( FAILED( m_ifcApiTouchPad ->CreateDevice( devHandle, &m_ifcDevTouchPad ) ) )
			return false;
			// Device error
		if ( FAILED( m_ifcDevTouchPad ->CreatePacket( &m_ifcPacketTouchPad ) ) ) 
			return false;
			// Device error
		m_ifcApiTouchPad ->Release( );
		m_ifcApiTouchPad = nullptr;

		// Set calllback. Goto "_ISynDeviceEvents::OnSynDevicePacket() override" with HWND=nullptr
		if ( FAILED( m_ifcDevTouchPad ->SetSynchronousNotification( this ) ) ) 
			return false;

		::EmptyWorkingSet( ::GetCurrentProcess( ) );
		return true;
	}
	void stop() {
		if ( m_ifcApiTouchPad ) {
			m_ifcApiTouchPad ->Release( );
			m_ifcApiTouchPad = nullptr;
		}
		if ( m_ifcDevTouchPad ) {
			m_ifcDevTouchPad ->Release( );
			m_ifcDevTouchPad = nullptr;
		}
		if ( m_ifcPacketTouchPad ) {
			m_ifcPacketTouchPad ->Release( );
			m_ifcPacketTouchPad = nullptr;
		}
		::CoUninitialize( );
	}

	// Mock VCL(Borland C++) part2
	TCheckBox *scrollAccEnabled;
	TCheckBox *scrollLinearEdge;
	int scrollMode;
	TRadioButton *scrollCompatible;
	TRadioButton *scrollSmooth;
	TTrackBar *scrollAcc;
	TTrackBar *scrollSpeed;
};
}}} // namespace prj_sysw::TinySynapticsScroll::Legacy _
#ifdef __clang__
#	pragma clang diagnostic pop
#endif // __clang__
