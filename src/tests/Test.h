#pragma once

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * A simple test class for a home-spun tiny test framework
 */

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include<format>

#define TEST( NAME ) cgm::Test* T__##NAME##__ = &(*(new cgm::Test{#NAME})).set([]() -> std::pair<bool, std::string>
// cgm::Test T__NAME__ =
#define EXCEPT( EXCNAME ) ).expect<EXCNAME>();
#define EXPECT( EXCNAME ) ).expect<EXCNAME>();
#define NOEXCEPT );

namespace cgm {
class Test {
  public:
	Test( std::string name );
	inline Test( Test const &rhs ) noexcept = delete;
	Test( Test && ) noexcept                = delete;

	template<typename T>
	    requires std::is_base_of_v<std::exception, T>
	inline Test &expect( ) {
		auto oldF = _f;

		_f = [oldF ]( ) -> std::pair<bool, std::string> {
			try {
				auto result = oldF( );
				return {
				  false,

				  std::format("expected exception of type '{}', but test returned ({} '{}')",
						  typeid( T ).name( ),
				    ( result.first ? "PASS" : "FAIL" ),
				    result.second
				     )};
			} catch( T const &expected_exception ) {
				return {
				  true,
					  std::format("caught exception of type '{}': {}",
				    typeid( T ).name( ),
				    expected_exception.what( )
				    ) };
			} catch( std::exception const &unexpected ) {
				return {
				  false,
				  std::format("expected exception of type '{}', but caught different exception of type '{}': {}:",
				    typeid( T ).name( ),
					typeid( unexpected ).name(),
				    unexpected.what( )
				    ) };
			}
		};
		return *this;
	}

	template<typename F>
	    requires std::is_invocable_v<F>
	inline Test &set( F f ) {
		_f = f;
		return *this;
	}

	inline std::pair<bool, std::string> run( ) const noexcept {
		try {
			return _f( );
		} catch( std::exception const &e ) {
			return { false, std::format("caught unexpected exception of type '{}': {}", typeid(e).name(), e.what( ) ) };
		}
	}

	inline std::string getName( ) const noexcept { return _name; }

  private:
	std::string _name;
	std::function<std::pair<bool, std::string>( )> _f;
};
}  // namespace cgm
