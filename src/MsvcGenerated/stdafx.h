// stdafx.h - STanDartApplicationFrameworkX/pch for system headers

// @insp https://stackoverflow.com/questions/5663091/programming-in-c-win-api-how-to-get-windows-7-look-for-controls/5663284#5663284
#ifndef __clang__
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif // _M_IX86
#endif // __clang__

// disable min and max macros
#define NOMINMAX
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif
#pragma warning( push )
#pragma warning( disable: 5039 )
#include "framework.h"
#pragma warning( pop )

#pragma comment( lib, "Comctl32" )
#pragma comment( lib, "ShlWApi" )

// Include Synaptics SDK headers
//	Define macro to disable include SynWraps.h file, because "error C4430: missing type specifier - int assumed. Note: C++ does not support default-int"
#define __SYNWRAPS_H
//	Search Synaptics SDK in internet -- use query "SynCOMAPIv1_0.zip", found "https://code.google.com/archive/p/aboutc/downloads?page=2", download link "https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/aboutc/SynCOMAPIv1_0.zip"
#pragma warning( push )
#pragma warning( disable: 28251 )
#pragma warning( disable: 4471 )
#include "C:\Prj\_L\SynCOMAPIv1_0\Include/SynKit.h"
#pragma warning( pop )
