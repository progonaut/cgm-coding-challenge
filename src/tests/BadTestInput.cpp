#include "BadTestInput.h"

#include <random>
#include<algorithm>

namespace cgm {
BadTestInput::BadTestInput( std::size_t nr_of_inputs_before_eof ) noexcept
    : _good_input{ nr_of_inputs_before_eof }, _max_nr_inputs{ nr_of_inputs_before_eof } {}

std::string BadTestInput::getLine( ) {
	if( _provided_nr_inputs >= _max_nr_inputs )
		return "";

	std::random_device dev;
	std::mt19937 rng( dev( ) );
	std::uniform_int_distribution<std::mt19937::result_type> dist( 0, _bad_input.size( ) - 1 );

	_provided_nr_inputs++;

	auto idx = dist( rng );

	if( idx % 3 != 0 ) {
		return _good_input.getLine( );
	} else {
		return _bad_input[dist( rng )].first;
	}
}

bool BadTestInput::eof( ) const noexcept {
	return _max_nr_inputs <= _provided_nr_inputs;
}

BadTestInput::Result BadTestInput::getResultFor( std::string input ) const noexcept {
	auto element = std::find_if(
	  _bad_input.begin( ), _bad_input.end( ), [&input]( auto element ) { return element.first == input; } );

	if( element != _bad_input.end( ) ) {
		return element->second;
	} else {
		return Result::Pass;
	}
}
}
