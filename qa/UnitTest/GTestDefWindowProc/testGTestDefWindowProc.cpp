#include "..\pch.h"

// There is user32.lib!_DefWindowProcA mapped/forward to ntdll.dll!_NtdllDefWindowProc_A
struct MockMethodGlobal {
	MOCK_METHOD(BOOL, DefWindowProcA_, (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) );
} *g_poMockMethodHolder;
#include "..\..\..\src\Tool\Trait.h"
#define DefWindowProcA(a0_, a1_, a2_, a3) \
	g_poMockMethodHolder ->DefWindowProcA_( a0_, a1_, a2_, a3 )
#include "..\..\..\src\ForwardToInstance.h"
#undef DefWindowProcA

#include "..\..\..\src\Main.h"

using testing::_;
using testing::Return;
using namespace prj_sysw::TinySynapticsScroll;

MOCK_STDCALL_FUNC(LONG, GetWindowLongA, HWND, int);

namespace Window {

class WndProcMockParrentSep_ : public ForwardToInstance::Base {
public:
	MOCK_METHOD(LRESULT, messagesHandler_, (UINT uMsg, WPARAM wParam, LPARAM lParam) );
	static constexpr auto cast_v = &CREATESTRUCTA::lpCreateParams;
	using parrent_t = ForwardToInstance::OfWindow< decltype( &messagesHandler_ ), &messagesHandler_, decltype( cast_v ), cast_v >;
};
struct WndProcMockSep : public WndProcMockParrentSep_, public WndProcMockParrentSep_::parrent_t 
{};

NAMESPACE_TEST(ForwardToInstance_, Window, tooEarly) {
	HWND m_hWnd = (HWND)1;
	LRESULT lrReturnValue = (LRESULT)0x777;
	EXPECT_MODULE_FUNC_CALL( GetWindowLongA, m_hWnd, GWLP_USERDATA )
		.WillOnce( Return( 0 ) );

	g_poMockMethodHolder = new std::decay_t< decltype( *g_poMockMethodHolder ) >;
    std::shared_ptr< void > finalAction(nullptr, [](auto&&...) {
			delete g_poMockMethodHolder;
		});
	EXPECT_CALL( *g_poMockMethodHolder, DefWindowProcA_( m_hWnd, WM_USER, 0, 0 ) )
		.Times( 1 )
		.WillOnce( Return( lrReturnValue ) );

	EXPECT_EQ( lrReturnValue, WndProcMockSep::systemCallback( m_hWnd, WM_USER, 0, 0 ) );
}

} // namespace Window _
