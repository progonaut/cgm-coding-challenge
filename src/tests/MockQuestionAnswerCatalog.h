
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * A "mock" of the default facade to tie together the various (tiny) modules
 * This mock differs primarily in that it does absolutely no error correction.
 *
 * It also uses an externally provided Catalog, such that a test could keep track of
 * the changes made.
 *
 * The MockQuestionAnswerCatalog is templated on the Input source to
 * allow for specialisations for specific inputs.
 */

#include "../InputParser.h"
#include "../InputProcessor.h"
#include "../InputProvider.h"

#include "BadTestInput.h"
#include "TestRunner.h"

#include<format>

namespace cgm {

template<typename INPUT_TYPE>
    requires std::is_base_of_v<InputProvider, INPUT_TYPE>
class MockQuestionAnswerCatalog {
  public:
	MockQuestionAnswerCatalog(
	  Catalog &catalog, INPUT_TYPE &input_provider, InputParser &input_parser, InputProcessor &input_processor )
	    : _input{ input_provider }, _parser{ input_parser }, _processor{ input_processor }, _catalog{ catalog } {}

	void execute( ) {
		cgm::TestNullStream blind_output;
		while( !_input.eof( ) ) {
			_processor.process( _parser.parse( _input.getLine( )), _catalog, blind_output );
		}
	}

  private:
	InputProvider &_input;
	InputParser &_parser;
	InputProcessor &_processor;
	Catalog &_catalog;
};

}  // namespace cgm


namespace cgm {
	// This template specialisation allows the use of "bad input" and knows how to deal with it in a productive
	// manner for the implementation of integration tests:
	//
	// * the output of the provided processor, given the bad input, the parser, and the catalog, 
	//   is compared to the expected output (kind of output)
	// * if the output, be it error or ok, matches the expected kind of output, the test continues
	// * otherwise, an error is thrown that fails the integration test
	//
template<>
class MockQuestionAnswerCatalog<BadTestInput> {
  public:
	MockQuestionAnswerCatalog(
	  Catalog &catalog, BadTestInput& input_provider, InputParser &input_parser, InputProcessor &input_processor )
	    : _input{ input_provider }, _parser{ input_parser }, _processor{ input_processor }, _catalog{ catalog } {}

	void execute( ) {
		bool error{false};
		std::string input;

		cgm::TestNullStream blind_output;

		while( !_input.eof( ) && !error) {
			try {
				input = _input.getLine();
				_processor.process( _parser.parse( input  ), _catalog, blind_output );

				if( _input.getResultFor( input ) != BadTestInput::Result::Pass ) {
					error = true;
				}
			} catch( std::exception const& e ) {
				if( _input.getResultFor( input ) != BadTestInput::Result::Fail ) {
					error = true;
				}
			}
		}

		if( error ) {
			throw std::invalid_argument{ std::format("Integration test with bad input failed when testing input '{}'; expected {}, but got {}!", input,
					(_input.getResultFor(input) == BadTestInput::Result::Fail? "FAIL":"PASS"),
					(_input.getResultFor(input) != BadTestInput::Result::Fail? "FAIL":"PASS")
					)};
		}
	}

  private:
	BadTestInput &_input;
	InputParser &_parser;
	InputProcessor &_processor;
	Catalog &_catalog;
};
}
