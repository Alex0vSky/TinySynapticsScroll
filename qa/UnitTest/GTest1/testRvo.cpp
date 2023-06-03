#include "pch.h"

namespace testRvo {

MOCKREG_DECL( MockRegistryRvo ) { MOCKREG_BODY( MockRegistryRvo )
	MOCK_METHOD(void, Dtor, ());
	~MockRegistryRvo() {
		Dtor( );
	}
};

NAMESPACE_TEST(Rvo_PersistSettings, MemberRegistry, Scroll) {
	auto pAutorun = new ScrollPublicMember< MockRegistryRvo >( nullptr );
	EXPECT_CALL( const_cast< MockRegistryRvo &> ( pAutorun ->m_oReg ), Dtor() );
	delete pAutorun;
}

NAMESPACE_TEST(Rvo_PersistSettings, MemberRegistry, Autorun) {
	auto pAutorun = new AutorunPublicMember< MockRegistryRvo >( IDC_CHECK_Launch_with_Windows );
	EXPECT_CALL( const_cast< MockRegistryRvo &> ( pAutorun ->m_oReg ), Dtor() );
	delete pAutorun;
}

} // namespace testRvo _
