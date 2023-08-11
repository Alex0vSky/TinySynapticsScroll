// src\Main.h - code main class, for unittests code coverage
#pragma once // Copyright 2023 Alex0vSky (https://github.com/Alex0vSky)
#include "resource.h" // NOLINT(build/include_subdir)
#include "Tool\ErrorHandler.h"
#include "Tool\HeapMem.h"
#include "Tool\Registry.h"
#include "Tool\Trait.h"
#include "Config.h"
#include "Legacy\SynapticTouchPad.h"
#include "WrapperTouchPad.h"
#include "RestoreTouchPadFromSuspend.h"
#include "PersistSettings\Scroll.h"
#include "PersistSettings\Autorun.h"
#include "ForwardToInstance.h"
#include "DialogPopupSettings.h"
#include "Menu.h"
#include "Tray.h"
#include "WndProc.h"
#include "CEmptyWorkingSet.h"
#include "Version.h"
namespace prj_sysw { namespace TinySynapticsScroll {
struct Main {
	static UINT run(HINSTANCE hInst) {
		// Objects chain
		WrapperTouchPad< > oTouchPad;
		if ( !oTouchPad.start( ) ) 
			return Tool::ErrorHandler::showMsg( "Start working with TouchPad" );
		PersistSettings::Scroll<> oSettingsScroll( &oTouchPad );
		if ( !oSettingsScroll.readConfigFromRegistry( ) ) 
			return Tool::ErrorHandler::showMsg( "Read or initialize config from registry" );
		DialogPopupSettings<> oDialog( &oSettingsScroll );
		Menu< > oMenu( &oDialog );
		Tray< > oTray( hInst );
		WndProc oWndProc( &oTouchPad, &oTray, &oMenu );
		if ( !oTray.init( WndProc::systemCallback, &oWndProc, oMenu ) )
			return Tool::ErrorHandler::showMsg( "SistemTray initialization" );

		CEmptyWorkingSet< > oEmptyWorkingSet( oTray, &oTouchPad );
		MSG stMsg = { };
		while ( ::GetMessageA( &stMsg, oTouchPad.getHwndForMessagesDispatch( oTray.getHwnd( ) ), 0, 0 ) ) {
			// Automatic translate and dispatch message to dialog by single call 
			// @insp https://devblogs.microsoft.com/oldnewthing/20131009-00/?p=2983
			if ( !::IsDialogMessageA( oDialog.getHandle( ), &stMsg ) ) {
				(void)::TranslateMessage( &stMsg ), ::DispatchMessageA( &stMsg );
				oEmptyWorkingSet.handleWindowMessage( stMsg );
			}
		}
		return static_cast< UINT >( stMsg.wParam );
	}
};
}} // namespace prj_sysw::TinySynapticsScroll _
