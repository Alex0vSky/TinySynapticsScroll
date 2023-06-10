//
// pch.cpp
//

#include "pch.h"

// Common WinApi defines
MOCK_DEFINE_FUNC( CheckDlgButton );
MOCK_DEFINE_FUNC( GetModuleFileNameW );
MOCK_DEFINE_FUNC( SetWindowLongPtrA );
MOCK_DEFINE_FUNC( GetWindowLongPtrA );
MOCK_DEFINE_FUNC( GetDlgItem );
MOCK_DEFINE_FUNC( DestroyWindow );
MOCK_DEFINE_FUNC( MessageBoxA );
MOCK_DEFINE_FUNC( SetTimer );
MOCK_DEFINE_FUNC( KillTimer );

