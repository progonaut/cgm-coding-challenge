#include"TestRunner.h"

#include <iomanip>
#include <iostream>
#include<cstddef>

#include"Test.h"

#if defined( RUNTESTS )
namespace cgm {
	TestRunner &TestRunner::get( ) noexcept {
		static TestRunner runner;
		return runner;
	}

	TestRunner &TestRunner::addTest( Test &t ) {
		_tests.push_back( &t );
		return *this;
	}

	TestRunner &TestRunner::delTest( Test &t ) {
		auto tp = &t;
		std::remove( _tests.begin(), _tests.end(), tp );
		return *this;
	}

	void TestRunner::run( ) noexcept try {
		std::size_t count_failure{0}, count_ok{0}, ignored_meta{0};

		std::cout << "Running " << _tests.size( ) << " tests." << std::endl;
		std::vector<std::pair<std::string, std::pair<bool, std::string>>> collected_results;

		TestNullStream blind_output;
		for( auto &t : _tests ) {
			std::cout << "[" << t->getName( ) << "]" << std::endl;
			auto test_result = std::make_pair( t->getName( ), t->run( blind_output )); 
			if( !test_result.first.starts_with("_META_") ) {
				if( test_result.second.first ) count_ok++;	
				else count_failure++;
			} else {
				ignored_meta++;
			}

			collected_results.push_back( std::move(test_result) );
		}

		std::cout << std::endl << "**** R E S U L T S ****" << std::endl;
		std::cout << "Of " << _tests.size() << " tests, " 
			<< std::setw(3) << count_ok << " PASSED, "
			<< std::setw(3) << count_failure << " FAILED, "
			<< std::setw(3) << ignored_meta << " were IGNORED (META-TESTS)." << std::endl << std::endl;


		for( auto const &r : collected_results ) {
			std::cout << std::setw( 80 ) << (cgm::verbose?std::left:std::right) << std::format( "TEST({})",r.first) << std::setw( 8 )
			          << ( r.second.first ? "OK" : "FAILED" );
			if( r.second.second != "" && cgm::verbose ) {
				std::cout << "\n\t - " << r.second.second;
			}
			if( cgm::verbose ) std::cout << "\n";
			std::cout << std::endl;
		}
	} catch( std::exception const &e ) {
		std::cerr << "TEST RUNNER CRITICAL FAILURE: Exception broke through guard: " << e.what( ) << "! Aborting ..."
		          << std::endl;
		throw;
	}
}
#endif
