#pragma once
#include "../pch.h"

MOCK_STDCALL_DECLARE_FUNC(LSTATUS, CheckDlgButton, HWND hDlg, int nIDButton, UINT uCheck);
MOCK_STDCALL_DECLARE_FUNC(LSTATUS, GetModuleFileNameW, HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
MOCK_STDCALL_DECLARE_FUNC(LONG_PTR, SetWindowLongPtrA, HWND hWnd, int nIndex, LONG_PTR dwNewLong);
MOCK_STDCALL_DECLARE_FUNC(LONG_PTR, GetWindowLongPtrA, HWND, int);
MOCK_STDCALL_DECLARE_FUNC(HWND, GetDlgItem, HWND hDlg, int nIDDlgItem );
MOCK_STDCALL_DECLARE_FUNC(BOOL, DestroyWindow, HWND hWnd);
MOCK_STDCALL_DECLARE_FUNC(int, MessageBoxA, HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
MOCK_STDCALL_DECLARE_FUNC(UINT_PTR, SetTimer, HWND hWnd, UINT_PTR nIDEvent, UINT uElapse,TIMERPROC lpTimerFunc);
MOCK_STDCALL_DECLARE_FUNC(BOOL, KillTimer, HWND hWnd, UINT_PTR uIDEvent);
MOCK_STDCALL_DECLARE_FUNC(ULONGLONG, GetTickCount64);

#define TRY_RESTORE_MODULE_FUNC(func) \
	if ( mock_module_##func::oldFn_ ) RESTORE_MODULE_FUNC( func )

namespace { 
	class Environment1 : public ::testing::Environment {
		void TearDown() override {
			TRY_RESTORE_MODULE_FUNC( CheckDlgButton );
			TRY_RESTORE_MODULE_FUNC( GetModuleFileNameW );
		}
	}; auto g_unusedPch = testing::AddGlobalTestEnvironment( new Environment1 ); 
}

#include "../../../src/Main.h"

//@insp https://stackoverflow.com/questions/28450803/copy-constructor-is-not-inherited/28450893#28450893
template<class T>
class MockRegistryCreator {
protected:
	MockRegistryCreator() = default;
public:
	MockRegistryCreator(const MockRegistryCreator &) = delete;
	MockRegistryCreator &operator = (const MockRegistryCreator &) = delete;
	MockRegistryCreator &operator = (const MockRegistryCreator &&) = delete;
	static T openAlways(prj_sysw::TinySynapticsScroll::Tool::Registry::EnuRootKey, const wchar_t *, prj_sysw::TinySynapticsScroll::Tool::Registry::EnuDesiredAccessRights) {
		return T( );
	}
};


// Better trick then inherit and "friend" to private, and parrent honstly declare protected member
template<class T>
struct AutorunPublicMember : public prj_sysw::TinySynapticsScroll::PersistSettings::Autorun< T > {
	using prj_sysw::TinySynapticsScroll::PersistSettings::Autorun< T >::Autorun;
	using prj_sysw::TinySynapticsScroll::PersistSettings::Autorun< T >::m_oReg;
};
template<class T,class T2=prj_sysw::TinySynapticsScroll::WrapperTouchPad<> >
struct ScrollPublicMember : public prj_sysw::TinySynapticsScroll::PersistSettings::Scroll< T,T2 > {
	using prj_sysw::TinySynapticsScroll::PersistSettings::Scroll< T,T2 >::Scroll;
	using prj_sysw::TinySynapticsScroll::PersistSettings::Scroll< T,T2 >::m_oReg;
};

struct MockWrapperTouchPad {
	MOCK_METHOD(void, enableAcceleration, (bool) );
	MOCK_METHOD(void, setValueAcceleration, (unsigned int) );
	MOCK_METHOD(void, enableKeepScrollingOnEdges, (bool) );
	MOCK_METHOD(void, setScrollModeSmooth, () );
	MOCK_METHOD(void, setScrollModeCompatible, () );
	MOCK_METHOD(void, setValueSpeed, (unsigned int) );
	MOCK_METHOD(bool, relaunch, () );
};


#pragma region
#define MOCKREG_DECL(clazz) \
	class clazz : public MockRegistryCreator< clazz > 
#define MOCKREG_BODY(clazz)						\
	clazz() = default;							\
	friend class MockRegistryCreator< clazz >;	\
public:											\
	clazz(const clazz &&)						\
	{}
#pragma endregion

MOCKREG_DECL( MockRegistryAutorun ) { MOCKREG_BODY( MockRegistryAutorun )
	MOCK_METHOD(bool, readString, (const wchar_t *, prj_sysw::TinySynapticsScroll::Tool::HeapMem *, size_t *), (const));
	MOCK_METHOD(bool, deleteValue, (const wchar_t *), (const));
	MOCK_METHOD(bool, createAndSetValueString, (const wchar_t *, const wchar_t *), (const));
};

MOCKREG_DECL( MockRegistryScroll ) { MOCKREG_BODY( MockRegistryScroll )
	MOCK_METHOD(bool, readDword, (const wchar_t *, DWORD *), (const));
	MOCK_METHOD(bool, createAndSetValueDword, (const wchar_t *, DWORD), (const));
};


using testing::_;
using ::testing::Return;
using namespace prj_sysw::TinySynapticsScroll;
