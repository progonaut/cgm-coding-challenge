#pragma once

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * A simple container for tests to register themselves and then be run if so desired
 */

#if defined( RUNTESTS )
#include <vector>
#include<format>

#include "Test.h"
#include "../globals.h"


namespace cgm {
class TestRunner {
	TestRunner( TestRunner const & ) = delete;
	TestRunner( TestRunner && )      = delete;
	~TestRunner( ) noexcept          = default;

  public:
	static TestRunner &get( ) noexcept;

	TestRunner &addTest( Test &t );

	void run( ) noexcept;

  private:
	std::vector<Test *> _tests;
	inline TestRunner( ) {};
};
}  // namespace cgm
#endif
