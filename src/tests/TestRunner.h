#pragma once

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * A simple container for tests to register themselves and then be run if so desired
 */

#if defined( RUNTESTS )
#include <vector>
#include<format>

#include "../globals.h"

#include<ostream>
namespace cgm {
class Test;

class TestNullStream : public std::ostream {
	public:
		inline TestNullStream() : std::ostream{nullptr} {}
		inline TestNullStream( TestNullStream const& ) : std::ostream{nullptr} { }
};

template<typename T>
TestNullStream& operator<<( TestNullStream& stream, T const& v ) {
	return stream;
}

class TestRunner {
	TestRunner( TestRunner const & ) = delete;
	TestRunner( TestRunner && )      = delete;
	~TestRunner( ) noexcept          = default;

  public:
	static TestRunner &get( ) noexcept;

	TestRunner &addTest( Test &t );
	TestRunner &delTest( Test &t );

	void run( ) noexcept;

  private:
	std::vector<Test *> _tests;
	inline TestRunner( ) {};
};
}  // namespace cgm
#endif
