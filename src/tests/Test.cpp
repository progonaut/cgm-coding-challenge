#include "Test.h"

#include "TestRunner.h"

namespace cgm {
Test::Test( std::string name ) : _name{ name }, _f {
	[name]( ) -> std::pair<bool, std::string> {
		return { false, std::format( "Test {} not correctly initialised.", name ) };
	}} {
#if defined( RUNTESTS )

		TestRunner::get( ).addTest( *this );
#endif
	}
}
