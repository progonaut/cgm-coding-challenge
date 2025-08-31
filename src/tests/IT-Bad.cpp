/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * IT-Bad is a simple integration test that uses random 
 * good input, interspersed with bad input, to test the entire system
 */

#include"Test.h"
#include"../globals.h"
#include"MockQuestionAnswerCatalog.h"
#include"BadTestInput.h"
#include"../DefaultParser.h"
#include"../DefaultProcessor.h"
#include<format>

TEST(IT_BAD_RANDOM_INPUT) {
	constexpr std::size_t input_count {10000};
	cgm::BadTestInput input_provider{input_count};
	cgm::DefaultParser input_parser;
	cgm::DefaultProcessor input_processor{ []( auto error_token ) {
		throw std::invalid_argument( std::format("Error: {}", error_token.second ) );
	} };

	cgm::Catalog catalog;

	cgm::MockQuestionAnswerCatalog<cgm::BadTestInput>{ catalog,input_provider, input_parser, input_processor }.execute();
	return {true, std::format("Completed {} inputs with input errors gracefully.", input_count )};
} NOEXCEPT
