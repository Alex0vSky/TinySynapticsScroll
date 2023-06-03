// src\Tool\ErrorHandler.h - little error handling and logging
#pragma once
namespace prj_sysw { namespace TinySynapticsScroll { namespace Tool { 
class ErrorHandler {
public:
	static UINT showMsg(const char *szText) {
		::MessageBoxA( NULL, szText, "TinySynapticsScroll error!", MB_ICONEXCLAMATION | MB_OK );
		return 0;
	}
	static void log(const char *szText) {
		::OutputDebugStringA( szText );
	}
};
}}} // namespace prj_sysw::TinySynapticsScroll::Tool _
