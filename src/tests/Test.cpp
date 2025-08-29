#include "Test.h"

#include "TestRunner.h"
#include"../util.h"

namespace cgm {
Test::Test( std::string name ) : _name{ name }, _f {
	[name]( ) -> std::pair<bool, std::string> {
		return { false, util::buildString( "Test ", name, " not correctly initialised." ) };
	}} {
#if defined( RUNTESTS )

		TestRunner::get( ).addTest( *this );
#endif
	}
}
