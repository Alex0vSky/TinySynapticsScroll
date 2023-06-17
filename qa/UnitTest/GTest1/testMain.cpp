#include "pch.h"

namespace { class Environment0 : public ::testing::Environment {
	void SetUp() override {
		// Take all working sources to *.pdb file for OpenCppCoverage
		if ( false ) prj_sysw::TinySynapticsScroll::Main::run( 0 );
	}
}; auto g_unusedMain = testing::AddGlobalTestEnvironment( new Environment0 ); }

// @insp https://stackoverflow.com/questions/71133972/is-there-a-way-to-detect-command-line-arguments-not-running-tests
namespace testing { namespace internal { extern bool g_help_flag; }} // namespace testing::internal _
// true if args not '--gtest_list_tests' and not --gtest_repeat=0' and not '--help'
bool RunAllTestsDoesRunTests() {
    return ( !::testing::GTEST_FLAG(list_tests) && ::testing::GTEST_FLAG(repeat) != 0 && !::testing::internal::g_help_flag );
}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	if ( RunAllTestsDoesRunTests( ) ) {
		std::cout << "Running main() from testMain.cpp" << std::endl;
	// Check OS. '_WIN32' -- both 32 bit and 64 bit
#if defined( _WIN32 ) || defined( __CYGWIN__ )
		std::string strPref = "[          ] ";
		std::cout << strPref << "check conflict (on first run Windows OS hooks from AppCompat conflicts with gmock-win32 hook)" << std::endl;
		try {
			void* funcAddr = &::DestroyWindow;
			// any not zero value
			void* newFunc = (void*)1;
			void* oldFunc = 0;
			mockModule_patchModuleFunc( funcAddr, newFunc, &oldFunc );
 			mockModule_restoreModuleFunc( funcAddr, newFunc, &oldFunc );
		} catch (std::runtime_error) {
			std::cerr << strPref << "conflict detected! Exit with error" << std::endl;
			// From macro RUN_ALL_TESTS: Returns 0 if successful, or 1 otherwise.
			return 1;
		}
#endif // defined( _WIN32 ) || defined( __CYGWIN__ )
	}
	return RUN_ALL_TESTS();
}
