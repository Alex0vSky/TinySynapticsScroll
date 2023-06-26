// src\TinySynapticsScroll.cpp - entry point. Icon: https://icon-icons.com/download/57949/ICO/32/
#include "MsvcGenerated\stdafx.h"
#include "Main.h"
#ifdef _DEBUG
int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int) {
    HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, NULL );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#else
int WINAPI WinMainCRTStartup(HINSTANCE, HINSTANCE, PWSTR, int) {
	HINSTANCE hInst = ::GetModuleHandleA( NULL );
#endif
	UINT uExitCode = prj_sysw::TinySynapticsScroll::Main::run( hInst );
#ifndef _DEBUG
	::ExitProcess( uExitCode );
#endif
	return static_cast< int >( uExitCode );
}
