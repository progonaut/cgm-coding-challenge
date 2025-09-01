#pragma once

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * A simple test class for a home-spun tiny test framework
 */

#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include<ostream>
#include"TestRunner.h"

#define TEST( NAME )      cgm::Test T__##NAME##__ = cgm::Test(#NAME, []( std::ostream& output ) -> std::pair<bool, std::string>
#define EXPECT( EXCNAME ) ).expect<EXCNAME>( )
#define PASS ).noexpect()

namespace cgm {
class Test {
  public:
	inline Test( std::string name, std::function<std::pair<bool, std::string>( std::ostream& )> f )
	    : _name{ name }, _f{ f } {}

	inline Test( Test const &rhs ) noexcept :_name{rhs._name}, _f{ rhs._f} {
#if defined( RUNTESTS )
		TestRunner::get( ).addTest( *this );
#endif
	}

	Test( Test && ) noexcept         = delete;

	inline ~Test() {
#if defined( RUNTESTS )
		TestRunner::get( ).delTest( *this );
#endif
	}
		

	template<typename T>
	    requires std::is_base_of_v<std::exception, T>
	inline Test &expect( ) {
		auto oldF = _f;

		_f = [oldF]( std::ostream& output ) -> std::pair<bool, std::string> {
			try {
				auto result = oldF( output );
				return {
				  false,

				  std::format(
				    "expected exception of type '{}', but test returned ({} '{}')",
				    typeid( T ).name( ),
				    ( result.first ? "PASS" : "FAIL" ),
				    result.second ) };
			} catch( T const &expected_exception ) {
				return {
				  true,
				  std::format( "caught exception of type '{}': {}", typeid( T ).name( ), expected_exception.what( ) ) };
			} catch( std::exception const &unexpected ) {
				return {
				  false,
				  std::format(
				    "expected exception of type '{}', but caught different exception of type '{}': {}:",
				    typeid( T ).name( ),
				    typeid( unexpected ).name( ),
				    unexpected.what( ) ) };
			}
		};
		return *this;
	}

	inline Test& noexpect() noexcept {
		return *this;
	}

	inline std::pair<bool, std::string> run( std::ostream& output ) const noexcept {
		try {
			return _f( output );
		} catch( std::exception const &e ) {
			return {
			  false, std::format( "caught unexpected exception of type '{}': {}", typeid( e ).name( ), e.what( ) ) };
		}
	}

	inline std::string getName( ) const { return _name; }

  private:
	std::string _name;
	std::function<std::pair<bool, std::string>( std::ostream& )> _f;
};
}  // namespace cgm
