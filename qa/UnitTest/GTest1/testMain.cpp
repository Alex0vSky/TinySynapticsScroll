#include "pch.h"

namespace { class Environment0 : public ::testing::Environment {
	void SetUp() override {
		// Take all working sources to *.pdb file for OpenCppCoveragePlugin
		if ( false ) prj_sysw::TinySynapticsScroll::Main::run( 0 );
	}
}; auto g_unusedMain = testing::AddGlobalTestEnvironment( new Environment0 ); }

static LRESULT CALLBACK wndProc(HWND hWndMsg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch( uMsg ) {
		case WM_DESTROY:
			::PostQuitMessage( 0 );
		break;
		default: 
			return ::DefWindowProcA( hWndMsg, uMsg, wParam, lParam );
	}
	return 0;
}
static bool WaitLoadingDlls() {
	HINSTANCE hInst = ::GetModuleHandleA( NULL );
	char szClassName[] = "TestHiddenWindowClass";
	const INITCOMMONCONTROLSEX stIccex = { sizeof( INITCOMMONCONTROLSEX ), ICC_UPDOWN_CLASS | ICC_LISTVIEW_CLASSES };
	if ( !::InitCommonControlsEx( &stIccex ) ) 
		return false;
	WNDCLASSEXA stWc = { };
	stWc.cbSize = sizeof( stWc ); 
	stWc.style = CS_HREDRAW | CS_VREDRAW; 
	stWc.lpfnWndProc = &wndProc;
	stWc.lpszClassName = szClassName;
	if ( !::RegisterClassExA( &stWc ) )
		return false;
	// Create the hidden window
	HWND m_hHiddenWnd = ::CreateWindowExA( 
			WS_EX_CLIENTEDGE 
			, szClassName, ""
			, WS_OVERLAPPEDWINDOW
			, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
			, NULL, NULL, hInst
			, NULL
		);
	if ( !m_hHiddenWnd ) 
		return false;
	if ( !::DestroyWindow( m_hHiddenWnd ) )
		return false;
	return true;
}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	// Wait loading dlls, for gmock-win32 patching
	std::cout << "Running main() from testMain.cpp" << std::endl;
	const unsigned int c_uTryCountMax = 20;
	unsigned int uCountTry = 0;
	std::string strPref = "[          ]";
	//for ( ; uCountTry < c_uTryCountMax; ++uCountTry ) {
	//	std::cout << strPref << " waiting import patching for gmock-win32, count: " << (1 + uCountTry) << std::endl;
	//	if ( WaitLoadingDlls( ) ) 
	//		break;
	//}
	for ( ; uCountTry < c_uTryCountMax; ++uCountTry ) {
		std::cout << strPref << " waiting import patching for gmock-win32, count: " << (1 + uCountTry) << std::endl;
		void* funcAddr = &::DestroyWindow;
		void* newFunc = (void*)1; // any not zero value
		void* oldFunc = 0;
		try {
			//throw std::runtime_error{ "failed to restore module function" };
			WaitLoadingDlls( );
			mockModule_patchModuleFunc( funcAddr, newFunc, &oldFunc );
 			mockModule_restoreModuleFunc( funcAddr, newFunc, &oldFunc );
			WaitLoadingDlls( );
		} catch (std::runtime_error) {
			::Sleep( 300 );
			continue;
		}
		break;
	}
	if ( uCountTry >= c_uTryCountMax ) {
		// From macro RUN_ALL_TESTS: Returns 0 if successful, or 1 otherwise.
		std::cerr << strPref << " fail waiting import patching for gmock-win32" << std::endl;
		return 1;
	}
	return RUN_ALL_TESTS();
}
