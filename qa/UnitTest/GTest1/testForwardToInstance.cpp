#include "pch.h"

// Get type MOCK_METHOD impl, disambiguate the overload @insp https://stackoverflow.com/questions/40135561/decltype-for-overloaded-member-function
#define DECL_MOCK_METHOD_ptmf_type( T, P, _MethodName, _TypeName )										\
	using _TypeName = decltype(																			\
		std::declval< T >( ).gmock_##_MethodName(														\
				std::declval< const ::testing::internal::WithoutMatchers & >( )							\
				, std::declval< ::testing::internal::Function< P > * >( )								\
			)																							\
	) (T::*)(const ::testing::internal::WithoutMatchers &, ::testing::internal::Function< P > *) const

namespace Dialog {

class DialogFuncMockParrent_ : public ForwardToInstance::Base {
public:
	HWND getHwnd() const {
		return m_hWnd;
	}
	MOCK_METHOD(INT_PTR, dialogFunc_, (UINT uMsg, WPARAM wParam, LPARAM lParam) );
	using parrent_t = ForwardToInstance::OfDialog< decltype( &DialogFuncMockParrent_::dialogFunc_ ), &DialogFuncMockParrent_::dialogFunc_ >;
};
struct DialogFuncMock : public DialogFuncMockParrent_, public DialogFuncMockParrent_::parrent_t 
{};

NAMESPACE_TEST(ForwardToInstance_, Dialog, tooEarly) {
	HWND m_hDialog = (HWND)2;
	EXPECT_MODULE_FUNC_CALL( GetWindowLongPtrA, m_hDialog, GWLP_USERDATA )
		.WillOnce( Return( 0 ) );
	EXPECT_FALSE( DialogFuncMock::systemCallback( m_hDialog, WM_USER, 0, 0 ) );
}
NAMESPACE_TEST(ForwardToInstance_, Dialog, init) {
	HWND m_hDialog = (HWND)2;
	DialogFuncMock oDialogFuncMock;
	LONG_PTR loDialogFuncMock = reinterpret_cast< LONG_PTR >( &oDialogFuncMock );
	EXPECT_MODULE_FUNC_CALL( SetWindowLongPtrA, m_hDialog, GWLP_USERDATA, loDialogFuncMock )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_MODULE_FUNC_CALL( GetWindowLongPtrA, m_hDialog, GWLP_USERDATA )
		.WillRepeatedly( Return( loDialogFuncMock ) );

	EXPECT_CALL( oDialogFuncMock, dialogFunc_( WM_INITDIALOG, _, LPARAM( &oDialogFuncMock ) ) )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );

	EXPECT_TRUE( DialogFuncMock::systemCallback( m_hDialog, WM_INITDIALOG, 0, LPARAM( &oDialogFuncMock ) ) );
	EXPECT_EQ( m_hDialog, oDialogFuncMock.getHwnd( ) );
}
NAMESPACE_TEST(ForwardToInstance_, Dialog, failSetWindowLongPtrA) {
	HWND m_hDialog = (HWND)2;
	DialogFuncMock oDialogFuncMock;
	LONG_PTR loDialogFuncMock = reinterpret_cast< LONG_PTR >( &oDialogFuncMock );
	EXPECT_MODULE_FUNC_CALL( SetWindowLongPtrA, m_hDialog, GWLP_USERDATA, loDialogFuncMock )
		.Times( 1 )
		.WillOnce( Return( FALSE ) );

	EXPECT_FALSE( DialogFuncMock::systemCallback( m_hDialog, WM_INITDIALOG, 0, LPARAM( &oDialogFuncMock ) ) );
	EXPECT_EQ( m_hDialog, oDialogFuncMock.getHwnd( ) );
}
} // namespace Dialog _


namespace Window {

class WndProcMockParrent_ : public ForwardToInstance::Base {
public:
	HWND getHwnd() const {
		return m_hWnd;
	}
	MOCK_METHOD(LRESULT, messagesHandler_, (UINT uMsg, WPARAM wParam, LPARAM lParam) );
	//static constexpr auto cast_v = &CREATESTRUCTA::lpCreateParams;
	//using parrent_t = ForwardToInstance::OfWindow< decltype( &WndProcMockParrent_::messagesHandler_ ), &WndProcMockParrent_::messagesHandler_, decltype( cast_v ), cast_v >;
	using parrent_t = ForwardToInstance::OfWindow< decltype( &WndProcMockParrent_::messagesHandler_ ), &WndProcMockParrent_::messagesHandler_, decltype( &CREATESTRUCTA::lpCreateParams ), &CREATESTRUCTA::lpCreateParams >;
};
struct WndProcMock : public WndProcMockParrent_, public WndProcMockParrent_::parrent_t 
{};

NAMESPACE_TEST(ForwardToInstance_, Window, init) {

	HWND m_hWnd = (HWND)1;
	CREATESTRUCTA m_lpCreateStructA{ };
	WndProcMock oWndProcMock;
	LONG_PTR loDialogFuncMock = reinterpret_cast< LONG_PTR >( &oWndProcMock );
	EXPECT_MODULE_FUNC_CALL( SetWindowLongPtrA, m_hWnd, GWLP_USERDATA, loDialogFuncMock )
		.Times( 1 )
		.WillOnce( Return( TRUE ) );
	EXPECT_MODULE_FUNC_CALL( GetWindowLongPtrA, m_hWnd, GWLP_USERDATA )
		.WillRepeatedly( Return( loDialogFuncMock ) );

	m_lpCreateStructA.lpCreateParams = &oWndProcMock;
	EXPECT_CALL( oWndProcMock, messagesHandler_( WM_NCCREATE, _, LPARAM( &m_lpCreateStructA ) ) )
		.Times( 1 )
		.WillOnce( Return( 0 ) );

	EXPECT_EQ( 0, WndProcMock::systemCallback( m_hWnd, WM_NCCREATE, 0, LPARAM( &m_lpCreateStructA ) ) );
	EXPECT_EQ( m_hWnd, oWndProcMock.getHwnd( ) );
}

} // namespace Window _
