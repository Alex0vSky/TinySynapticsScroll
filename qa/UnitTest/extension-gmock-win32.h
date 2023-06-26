#pragma once // Accept more then 8 args, for WinApi functions like RegCreateKeyExW() //@insp https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
// Allow up to 62 args
#   define GET_ARG_COUNT(...)  INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))
#		define INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#		define INTERNAL_EXPAND(x) x
#		define INTERNAL_EXPAND_ARGS_PRIVATE(...) INTERNAL_EXPAND(INTERNAL_GET_ARG_COUNT_PRIVATE(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#		define INTERNAL_GET_ARG_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count
#undef MOCK_MODULE_NBARG
#define MOCK_MODULE_NBARG(...) GET_ARG_COUNT(__VA_ARGS__)


#pragma region DeclareStruct

#ifdef    MOCK_MODULE_FUNC0_
#undef    MOCK_MODULE_FUNC0_
#endif // MOCK_MODULE_FUNC0_
#ifdef    MOCK_MODULE_FUNC1_
#undef    MOCK_MODULE_FUNC1_
#endif // MOCK_MODULE_FUNC1_
#ifdef    MOCK_MODULE_FUNC2_
#undef    MOCK_MODULE_FUNC2_
#endif // MOCK_MODULE_FUNC2_
#ifdef    MOCK_MODULE_FUNC3_
#undef    MOCK_MODULE_FUNC3_
#endif // MOCK_MODULE_FUNC3_
#ifdef    MOCK_MODULE_FUNC4_
#undef    MOCK_MODULE_FUNC4_
#endif // MOCK_MODULE_FUNC4_
#ifdef    MOCK_MODULE_FUNC5_
#undef    MOCK_MODULE_FUNC5_
#endif // MOCK_MODULE_FUNC5_
#ifdef    MOCK_MODULE_FUNC6_
#undef    MOCK_MODULE_FUNC6_
#endif // MOCK_MODULE_FUNC6_
#ifdef    MOCK_MODULE_FUNC7_
#undef    MOCK_MODULE_FUNC7_
#endif // MOCK_MODULE_FUNC7_
#ifdef    MOCK_MODULE_FUNC8_
#undef    MOCK_MODULE_FUNC8_
#endif // MOCK_MODULE_FUNC8_
#ifdef    MOCK_MODULE_FUNC9_
#undef    MOCK_MODULE_FUNC9_
#endif // MOCK_MODULE_FUNC9_

#define MOCK_MODULE_FUNC0_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func() \
        constness \
    { \
        GMOCK_MOCKER_(0, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(0, constness, func).Invoke(); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func() \
    { \
        GMOCK_MOCKER_(0, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(0, constness, func).With(); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(0, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub() \
    { \
        return mock_module_##func::instance().func(); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC1_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1) constness \
    { \
        GMOCK_MOCKER_(1, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(1, constness, func).Invoke( \
            gmock_a1); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1) constness \
    { \
        GMOCK_MOCKER_(1, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(1, constness, func).With( \
            gmock_a1); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(1, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC2_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2) constness \
    { \
        GMOCK_MOCKER_(2, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(2, constness, func).Invoke( \
            gmock_a1, gmock_a2); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2) constness \
    { \
        GMOCK_MOCKER_(2, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(2, constness, func).With( \
            gmock_a1, gmock_a2); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(2, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC3_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3) constness \
    { \
        GMOCK_MOCKER_(3, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(3, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3) constness \
    { \
        GMOCK_MOCKER_(3, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(3, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(3, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC4_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4) constness \
    { \
        GMOCK_MOCKER_(4, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(4, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4) constness \
    { \
        GMOCK_MOCKER_(4, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(4, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(4, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC5_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5) constness \
    { \
        GMOCK_MOCKER_(5, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(5, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5) constness \
    { \
        GMOCK_MOCKER_(5, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(5, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(5, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC6_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6) constness \
    { \
        GMOCK_MOCKER_(6, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(6, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6) constness \
    { \
        GMOCK_MOCKER_(6, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(6, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(6, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC7_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
        GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7) constness \
    { \
        GMOCK_MOCKER_(7, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(7, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6, \
        GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7) constness \
    { \
        GMOCK_MOCKER_(7, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(7, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(7, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
        GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC8_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
        GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, \
        GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8) constness \
    { \
        GMOCK_MOCKER_(8, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(8, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
        GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6, \
        GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7, \
        GMOCK_MATCHER_(tn, 8, __VA_ARGS__) gmock_a8) constness \
    { \
        GMOCK_MOCKER_(8, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(8, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(8, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
        GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
        GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
        GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
        GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
        GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
        GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
        GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, \
        GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )


// New
#define MOCK_MODULE_FUNC9_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
          GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1 \
        , GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2 \
        , GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3 \
        , GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4 \
        , GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5 \
        , GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6 \
        , GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7 \
        , GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8 \
        , GMOCK_ARG_(tn, 9, __VA_ARGS__) gmock_a9 \
	) constness \
    { \
        GMOCK_MOCKER_(9, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(9, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
          GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1 \
        , GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2 \
        , GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3 \
        , GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4 \
        , GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5 \
        , GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6 \
        , GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7 \
        , GMOCK_MATCHER_(tn, 8, __VA_ARGS__) gmock_a8 \
        , GMOCK_MATCHER_(tn, 9, __VA_ARGS__) gmock_a9 \
	) constness \
    { \
        GMOCK_MOCKER_(9, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(9, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(9, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub( \
          GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1 \
        , GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2 \
        , GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3 \
        , GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4 \
        , GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5 \
        , GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6 \
        , GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7 \
        , GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8 \
        , GMOCK_ARG_(tn, 9, __VA_ARGS__) gmock_a9 \
	) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9 \
		); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC9(m, ...) MOCK_MODULE_FUNC9_(, , , m, __VA_ARGS__)
#define MOCK_MODULE_FUNC9_CALLCONV(ct, m, ...) MOCK_MODULE_FUNC9_(, , ct, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC9_STDCALL_CONV(m, ...) MOCK_MODULE_FUNC9_CALLCONV(__stdcall, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC9_CDECL_CONV(m, ...) MOCK_MODULE_FUNC9_CALLCONV(__cdecl, m, __VA_ARGS__)

#define MOCK_MODULE_FUNC10_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
          GMOCK_ARG_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_ARG_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_ARG_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_ARG_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_ARG_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_ARG_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_ARG_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_ARG_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_ARG_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10 \
	) constness \
    { \
        GMOCK_MOCKER_(10, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(10, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
          GMOCK_MATCHER_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_MATCHER_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_MATCHER_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_MATCHER_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_MATCHER_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_MATCHER_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_MATCHER_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_MATCHER_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_MATCHER_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_MATCHER_(tn, 10, __VA_ARGS__) gmock_a10 \
	) constness \
    { \
        GMOCK_MOCKER_(10, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(10, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(10, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub(  \
          GMOCK_ARG_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_ARG_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_ARG_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_ARG_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_ARG_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_ARG_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_ARG_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_ARG_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_ARG_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10 \
	) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10 \
		); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC10(m, ...) MOCK_MODULE_FUNC10_(, , , m, __VA_ARGS__)
#define MOCK_MODULE_FUNC10_CALLCONV(ct, m, ...) MOCK_MODULE_FUNC10_(, , ct, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC10_STDCALL_CONV(m, ...) MOCK_MODULE_FUNC10_CALLCONV(__stdcall, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC10_CDECL_CONV(m, ...) MOCK_MODULE_FUNC10_CALLCONV(__cdecl, m, __VA_ARGS__)

#define MOCK_MODULE_FUNC11_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
          GMOCK_ARG_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_ARG_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_ARG_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_ARG_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_ARG_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_ARG_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_ARG_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_ARG_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_ARG_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10 \
        , GMOCK_ARG_(tn, 11, __VA_ARGS__) gmock_a11 \
	) constness \
    { \
        GMOCK_MOCKER_(11, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(11, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10, gmock_a11); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
          GMOCK_MATCHER_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_MATCHER_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_MATCHER_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_MATCHER_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_MATCHER_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_MATCHER_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_MATCHER_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_MATCHER_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_MATCHER_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_MATCHER_(tn, 10, __VA_ARGS__) gmock_a10 \
        , GMOCK_MATCHER_(tn, 11, __VA_ARGS__) gmock_a11 \
	) constness \
    { \
        GMOCK_MOCKER_(11, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(11, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10, gmock_a11); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(11, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub(  \
          GMOCK_ARG_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_ARG_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_ARG_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_ARG_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_ARG_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_ARG_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_ARG_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_ARG_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_ARG_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10 \
        , GMOCK_ARG_(tn, 11, __VA_ARGS__) gmock_a11 \
	) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10, gmock_a11 \
		); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC11(m, ...) MOCK_MODULE_FUNC11_(, , , m, __VA_ARGS__)
#define MOCK_MODULE_FUNC11_CALLCONV(ct, m, ...) MOCK_MODULE_FUNC11_(, , ct, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC11_STDCALL_CONV(m, ...) MOCK_MODULE_FUNC11_CALLCONV(__stdcall, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC11_CDECL_CONV(m, ...) MOCK_MODULE_FUNC11_CALLCONV(__cdecl, m, __VA_ARGS__)

#define MOCK_MODULE_FUNC12_(tn, constness, ct, func, ...) \
_Pragma( "optimize( \"\", off )" ) \
struct mock_module_##func \
{ \
    static void hideCodeFromOptimizer() { \
		if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
			mockModule_patchModuleFunc( \
				  &::func \
				, reinterpret_cast< void* >( &MOCK_MODULE_DEPAREN(func)::stub ) \
				, &MOCK_MODULE_DEPAREN(func)::oldFn_); \
	} \
    GMOCK_RESULT_(tn, __VA_ARGS__) ct func( \
          GMOCK_ARG_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_ARG_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_ARG_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_ARG_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_ARG_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_ARG_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_ARG_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_ARG_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_ARG_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10 \
        , GMOCK_ARG_(tn, 11, __VA_ARGS__) gmock_a11 \
        , GMOCK_ARG_(tn, 12, __VA_ARGS__) gmock_a12 \
	) constness \
    { \
        GMOCK_MOCKER_(12, constness, func).SetOwnerAndName(this, #func); \
        return GMOCK_MOCKER_(12, constness, func).Invoke( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10, gmock_a11, gmock_a12); \
    } \
    ::testing::MockSpec<__VA_ARGS__> gmock_##func( \
          GMOCK_MATCHER_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_MATCHER_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_MATCHER_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_MATCHER_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_MATCHER_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_MATCHER_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_MATCHER_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_MATCHER_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_MATCHER_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_MATCHER_(tn, 10, __VA_ARGS__) gmock_a10 \
        , GMOCK_MATCHER_(tn, 11, __VA_ARGS__) gmock_a11 \
        , GMOCK_MATCHER_(tn, 12, __VA_ARGS__) gmock_a12 \
	) constness \
    { \
        GMOCK_MOCKER_(12, constness, func).RegisterOwner(this); \
        return GMOCK_MOCKER_(12, constness, func).With( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10, gmock_a11, gmock_a12); \
    } \
    mutable ::testing::FunctionMocker<__VA_ARGS__> \
        GMOCK_MOCKER_(12, constness, func); \
    static mock_module_##func& instance() \
    { \
        static ::testing::NiceMock< mock_module_##func > obj; \
        return obj; \
    } \
    static GMOCK_RESULT_(tn, __VA_ARGS__) ct stub(  \
          GMOCK_ARG_(tn, 1,  __VA_ARGS__) gmock_a1  \
        , GMOCK_ARG_(tn, 2,  __VA_ARGS__) gmock_a2  \
        , GMOCK_ARG_(tn, 3,  __VA_ARGS__) gmock_a3  \
        , GMOCK_ARG_(tn, 4,  __VA_ARGS__) gmock_a4  \
        , GMOCK_ARG_(tn, 5,  __VA_ARGS__) gmock_a5  \
        , GMOCK_ARG_(tn, 6,  __VA_ARGS__) gmock_a6  \
        , GMOCK_ARG_(tn, 7,  __VA_ARGS__) gmock_a7  \
        , GMOCK_ARG_(tn, 8,  __VA_ARGS__) gmock_a8  \
        , GMOCK_ARG_(tn, 9,  __VA_ARGS__) gmock_a9  \
        , GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10 \
        , GMOCK_ARG_(tn, 11, __VA_ARGS__) gmock_a11 \
        , GMOCK_ARG_(tn, 12, __VA_ARGS__) gmock_a12 \
	) \
    { \
        return mock_module_##func::instance().func( \
            gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10, gmock_a11, gmock_a12 \
		); \
    } \
    static void* oldFn_; \
}; \
_Pragma( "optimize( \"\", on )" )

#define MOCK_MODULE_FUNC12(m, ...) MOCK_MODULE_FUNC12_(, , , m, __VA_ARGS__)
#define MOCK_MODULE_FUNC12_CALLCONV(ct, m, ...) MOCK_MODULE_FUNC12_(, , ct, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC12_STDCALL_CONV(m, ...) MOCK_MODULE_FUNC12_CALLCONV(__stdcall, m, __VA_ARGS__)
#define MOCK_MODULE_FUNC12_CDECL_CONV(m, ...) MOCK_MODULE_FUNC12_CALLCONV(__cdecl, m, __VA_ARGS__)

// clang
#ifdef    MOCK_STDCALL_FUNC
#undef    MOCK_STDCALL_FUNC
#endif
#define MOCK_STDCALL_FUNC(r, m, ...) MOCK_MODULE_OVERLOAD(MOCK_MODULE_FUNC,MOCK_MODULE_NBARG(__VA_ARGS__)_STDCALL_CONV)(m, r(__VA_ARGS__))

#ifdef    MOCK_CDECL_FUNC
#undef    MOCK_CDECL_FUNC
#endif
#define MOCK_CDECL_FUNC(r, m, ...) MOCK_MODULE_OVERLOAD(MOCK_MODULE_FUNC,MOCK_MODULE_NBARG(__VA_ARGS__)_CDECL_CONV)(m, r(__VA_ARGS__))

#ifdef    MOCK_MODULE_FUNC
#undef    MOCK_MODULE_FUNC
#endif
#define MOCK_MODULE_FUNC(r, m, ...) MOCK_MODULE_OVERLOAD(MOCK_MODULE_FUNC,MOCK_MODULE_NBARG(__VA_ARGS__))(m, r(__VA_ARGS__))

#ifdef    MOCK_MODULE_PREFIX
#undef    MOCK_MODULE_PREFIX
#endif
#define MOCK_MODULE_PREFIX(...) 0, __VA_ARGS__

#pragma endregion 


// allow expand defined GetWindowLongPtrA to real GetWindowLongA on Win32
// @insp https://stackoverflow.com/questions/24481810/how-to-remove-the-enclosing-parentheses-with-macro
// prevent optimizing calls WinApi
// @insp https://learn.microsoft.com/en-us/cpp/preprocessor/pragma-directives-and-the-pragma-keyword
#define MOCK_MODULE_ESC(...) MOCK_MODULE_ESC_(__VA_ARGS__)
#define MOCK_MODULE_ESC_(...) mock_module_ ## __VA_ARGS__
#define MOCK_MODULE_DEPAREN(X) MOCK_MODULE_ESC(X)
#ifdef    EXPECT_MODULE_FUNC_CALL
#undef    EXPECT_MODULE_FUNC_CALL
#endif // EXPECT_MODULE_FUNC_CALL
#define EXPECT_MODULE_FUNC_CALL(func, ...) \
    MOCK_MODULE_DEPAREN(func)::hideCodeFromOptimizer( ); \
    EXPECT_CALL(MOCK_MODULE_DEPAREN(func)::instance(), func(__VA_ARGS__))

#ifdef    ON_MODULE_FUNC_CALL
#undef    ON_MODULE_FUNC_CALL
#endif // ON_MODULE_FUNC_CALL
#define ON_MODULE_FUNC_CALL(func, ...) \
    if (!MOCK_MODULE_DEPAREN(func)::oldFn_) \
    { \
        mockModule_patchModuleFunc(&func, reinterpret_cast< void* >( \
            &MOCK_MODULE_DEPAREN(func)::stub), &MOCK_MODULE_DEPAREN(func)::oldFn_); \
    } \
    ON_CALL(MOCK_MODULE_DEPAREN(func)::instance(), func(__VA_ARGS__))


#ifdef    INVOKE_REAL_MODULE_FUNC
#undef    INVOKE_REAL_MODULE_FUNC
#endif // INVOKE_REAL_MODULE_FUNC
#define INVOKE_REAL_MODULE_FUNC(func, ...) \
    reinterpret_cast< decltype(&func) >(MOCK_MODULE_DEPAREN(func)::oldFn_)(__VA_ARGS__)


#ifdef    VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS
#undef    VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS
#endif // VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS
#define VERIFY_AND_CLEAR_MODULE_FUNC_EXPECTATIONS(func) \
    ::testing::Mock::VerifyAndClearExpectations(&MOCK_MODULE_DEPAREN(func)::instance())


#ifdef    RESTORE_MODULE_FUNC
#undef    RESTORE_MODULE_FUNC
#endif // RESTORE_MODULE_FUNC
#define RESTORE_MODULE_FUNC(func) \
    mockModule_restoreModuleFunc(MOCK_MODULE_DEPAREN(func)::oldFn_, MOCK_MODULE_DEPAREN(func)::stub, &MOCK_MODULE_DEPAREN(func)::oldFn_)


#ifdef    MOCK_STDCALL_FUNC
#undef    MOCK_STDCALL_FUNC
#endif // MOCK_STDCALL_FUNC
// TODO: avoid to directly use WinApi like GetLastError in gmock-win32\src\gmock-win32.cpp, cause: conflicts with user hooks
#define MOCK_STDCALL_FUNC(r, m, ...) \
	MOCK_STDCALL_DECLARE_FUNC(r, m, __VA_ARGS__); \
	MOCK_DEFINE_FUNC( m )


#define MOCK_MODULE_CONCAT_clang(x, y, z) x ## y ## z
#define MOCK_MODULE_OVERLOAD_clang(name, count, suff) MOCK_MODULE_CONCAT_clang(name, count, suff)
#define MOCK_STDCALL_DECLARE_FUNC(r, m, ...) \
	MOCK_MODULE_OVERLOAD_clang( MOCK_MODULE_FUNC, MOCK_MODULE_NBARG(__VA_ARGS__), _STDCALL_CONV)(m, r(__VA_ARGS__))
#define MOCK_DEFINE_FUNC(func) \
	void* MOCK_MODULE_DEPAREN(func)::oldFn_ = nullptr;
