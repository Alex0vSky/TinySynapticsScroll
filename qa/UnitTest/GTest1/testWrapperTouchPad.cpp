#include "pch.h"

namespace testWrapperTouchPad {

struct MockLegacySynapticTouchPad : public Legacy::SynapticTouchPad { 
	MOCK_METHOD(bool, run, () );
	MOCK_METHOD(void, stop, () );
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollAccEnabled;
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollLinearEdge;

	struct { 
		DWORD m_dwValue;
		auto operator=(DWORD dwValue) { 
			m_dwValue = dwValue;
			Assign( dwValue );
			return this;
		}
		MOCK_METHOD(void, Assign, (DWORD) );
	} scrollMode;
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollCompatible;
	struct { 
		bool Checked; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollSmooth;

	struct { unsigned int Position; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollAcc;
	struct { unsigned int Position; auto operator ->() { RightArrow( ); return this; }; MOCK_METHOD(void, RightArrow, (), () );
	} scrollSpeed;
};

struct WrapperTouchPadPublicMember : public WrapperTouchPad< MockLegacySynapticTouchPad > {
	using WrapperTouchPad::WrapperTouchPad;
	using WrapperTouchPad::m_oSynapticTouchPad;
};

class Context : public ::testing::Test {
protected:
	//m_oSynapticTouchPad;
	WrapperTouchPadPublicMember m_oWrapper;
	//::testing::NiceMock< MockWrapperTouchPad > m_oMockWrapperTouchPad;
	//ScrollPublicMember< MockRegistryScroll, MockWrapperTouchPad > m_oScroll;
	decltype( m_oWrapper.m_oSynapticTouchPad ) *m_poLegacy;
	Context() 
		: m_poLegacy( &m_oWrapper.m_oSynapticTouchPad )
	{}
};

typedef Context Initial;
NAMESPACE_TEST_F(WrapperTouchPad_, Initial, start) {
	EXPECT_CALL( m_oWrapper.m_oSynapticTouchPad, run( ) )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_oWrapper.start( ) );
}
NAMESPACE_TEST_F(WrapperTouchPad_, Initial, relaunch) {
	EXPECT_CALL( m_oWrapper.m_oSynapticTouchPad, stop( ) );
	EXPECT_CALL( m_oWrapper.m_oSynapticTouchPad, run( ) )
		.WillOnce( Return( true ) );
	EXPECT_TRUE( m_oWrapper.relaunch( ) );
}

typedef Context Manage;
NAMESPACE_TEST_F(WrapperTouchPad_, Manage, enable) {
	EXPECT_CALL( m_poLegacy ->scrollAccEnabled, RightArrow( ) );
	m_oWrapper.enableAcceleration( true );
	EXPECT_TRUE( m_poLegacy ->scrollAccEnabled.Checked );
	EXPECT_CALL( m_poLegacy ->scrollAccEnabled, RightArrow( ) );
	m_oWrapper.enableAcceleration( false );
	EXPECT_FALSE( m_poLegacy ->scrollAccEnabled.Checked );

	EXPECT_CALL( m_poLegacy ->scrollLinearEdge, RightArrow( ) );
	m_oWrapper.enableKeepScrollingOnEdges( true );
	EXPECT_TRUE( m_poLegacy ->scrollLinearEdge.Checked );
	EXPECT_CALL( m_poLegacy ->scrollLinearEdge, RightArrow( ) );
	m_oWrapper.enableKeepScrollingOnEdges( false );
	EXPECT_FALSE( m_poLegacy ->scrollLinearEdge.Checked );

	EXPECT_CALL( m_poLegacy ->scrollMode, Assign( Config::Movement::Mode::c_dwSmooth ) );
	EXPECT_CALL( m_poLegacy ->scrollCompatible, RightArrow( ) )
		.Times( ::testing::AtLeast( 1 ) );
	EXPECT_CALL( m_poLegacy ->scrollSmooth, RightArrow( ) )
		.Times( ::testing::AtLeast( 1 ) );
	m_oWrapper.setScrollModeSmooth( );
	EXPECT_FALSE( m_poLegacy ->scrollCompatible.Checked );
	EXPECT_TRUE( m_poLegacy ->scrollSmooth.Checked );
	EXPECT_EQ( Config::Movement::Mode::c_dwSmooth, m_poLegacy ->scrollMode.m_dwValue );

	EXPECT_CALL( m_poLegacy ->scrollMode, Assign( Config::Movement::Mode::c_dwCompatible ) );
	EXPECT_CALL( m_poLegacy ->scrollCompatible, RightArrow( ) )
		.Times( ::testing::AtLeast( 1 ) );
	EXPECT_CALL( m_poLegacy ->scrollSmooth, RightArrow( ) )
		.Times( ::testing::AtLeast( 1 ) );
	m_oWrapper.setScrollModeCompatible( );
	EXPECT_TRUE( m_poLegacy ->scrollCompatible.Checked );
	EXPECT_FALSE( m_poLegacy ->scrollSmooth.Checked );
	EXPECT_EQ( Config::Movement::Mode::c_dwCompatible, m_poLegacy ->scrollMode.m_dwValue );

	unsigned int uValue = 0x555;
	EXPECT_CALL( m_poLegacy ->scrollAcc, RightArrow( ) );
	m_oWrapper.setValueAcceleration( uValue );
	EXPECT_EQ( uValue, m_poLegacy ->scrollAcc.Position );

	EXPECT_CALL( m_poLegacy ->scrollSpeed, RightArrow( ) );
	m_oWrapper.setValueSpeed( uValue );
	EXPECT_EQ( uValue, m_poLegacy ->scrollSpeed.Position );
}

NAMESPACE_TEST_F(WrapperTouchPad_, Context, WndHandleForMessagesDispatch) {
	EXPECT_EQ( nullptr, m_oWrapper.getHwndForMessagesDispatch( (HWND)0 ) );
}

} // namespace testWrapperTouchPad _
