#include "..\pch.h"

// Is K32EmptyWorkingSet/EmptyWorkingSet has been imported without "__declspec(dllimport)" and function address placed in current *.exe file
static struct {
	MOCK_METHOD(BOOL, EmptyWorkingSet_, (HANDLE) );
} *g_poMockMethodHolder;
#ifdef EmptyWorkingSet
#	pragma push_macro( "EmptyWorkingSet" )
#	define TEST_NEED_POP
#	undef EmptyWorkingSet
#endif
#define EmptyWorkingSet(hProcess) \
	g_poMockMethodHolder ->EmptyWorkingSet_( hProcess )
//#include "../../../resource/resource.h"
//class WndProc;
//#include "../../../src/Tray.h"
#include "../../../src/Tool/ErrorHandler.h"
#include "../../../src/Legacy/SynapticTouchPad.h"
#include "../../../src/WrapperTouchPad.h"
#include "../../../src/CEmptyWorkingSet.h"
#undef EmptyWorkingSet
#ifdef TEST_NEED_POP
#	pragma pop_macro( "EmptyWorkingSet" )
#	undef TEST_NEED_POP
#endif

using testing::_;
using ::testing::Return;
using namespace prj_sysw::TinySynapticsScroll;

MOCK_STDCALL_FUNC(UINT_PTR, SetTimer, HWND hWnd, UINT_PTR nIDEvent, UINT uElapse,TIMERPROC lpTimerFunc);

namespace testCEmptyWorkingSet {

struct MockTray {
	MOCK_METHOD(HWND, getHwnd, (), (const) );
};
struct TouchPad {
	MOCK_METHOD(void, getLastPacketTickCount, (ULONGLONG *), (const) );
};

class ContextBeforeCtor_ : public ::testing::Test {
protected:
	static const DWORD c_dwClearMemTimerId = 1;
	const HWND m_hWnd = (HWND)31;
	MockTray m_oMockTray;
	TouchPad m_oTouchPad;
	ContextBeforeCtor_()
	{
		EXPECT_CALL( m_oMockTray, getHwnd( ) )
			.WillOnce( Return( m_hWnd ) );
		EXPECT_MODULE_FUNC_CALL( SetTimer, m_hWnd, c_dwClearMemTimerId, _, nullptr );
	}
};
class Context : public ContextBeforeCtor_ {
protected:
	CEmptyWorkingSet< MockTray, TouchPad > m_oCEmptyWorkingSet;
	Context() 
		: m_oCEmptyWorkingSet( m_oMockTray, &m_oTouchPad )
	{}
	void SetUp() override {
		VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS( SetTimer );
	}
};

typedef Context WM_TIMER_;
NAMESPACE_TEST_F(CEmptyWorkingSet_HandleWindowMessage, WM_TIMER_, onlyOne) {
	g_poMockMethodHolder = new std::decay_t< decltype( *g_poMockMethodHolder ) >;
    std::shared_ptr< void > finalAction(nullptr, [](auto&&...) {
			delete g_poMockMethodHolder;
		});
	EXPECT_CALL( *g_poMockMethodHolder, EmptyWorkingSet_( ::GetCurrentProcess( ) ) );
	ULONGLONG ullOut;
	EXPECT_CALL( m_oTouchPad, getLastPacketTickCount( _ ) );

	MSG stMsg;
	stMsg.message = WM_TIMER;
	stMsg.wParam = c_dwClearMemTimerId;
	stMsg.hwnd = m_hWnd;
	m_oCEmptyWorkingSet.handleWindowMessage( stMsg );
}

} // namespace testCEmptyWorkingSet _
