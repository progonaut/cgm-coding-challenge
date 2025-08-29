#include "GoodTestInput.h"

#include <random>

namespace cgm {
GoodTestInput::GoodTestInput( std::size_t nr_of_inputs_before_eof ) noexcept
    : _max_nr_inputs{ nr_of_inputs_before_eof } {}

std::string GoodTestInput::getLine( ) {
	if( _provided_nr_inputs >= _max_nr_inputs ) return "";

	std::random_device dev;
	std::mt19937 rng( dev( ) );
	std::uniform_int_distribution<std::mt19937::result_type> dist( 0, _good_input.size()-1 ); 

	_provided_nr_inputs++;
	return _good_input[dist(rng)];
}

bool GoodTestInput::eof( ) const noexcept {
	return _max_nr_inputs <= _provided_nr_inputs;
}
}
