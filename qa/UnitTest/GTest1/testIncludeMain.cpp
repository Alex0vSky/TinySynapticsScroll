#include "pch.h"

namespace { class Environment0 : public ::testing::Environment {
	void SetUp() override {
		// Take all working sources to *.pdb file for OpenCppCoveragePlugin
		if ( false ) prj_sysw::TinySynapticsScroll::Main::run( 0 );
	}
}; auto g_unusedMain = testing::AddGlobalTestEnvironment( new Environment0 ); }
