/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * IT-Good is a simple integration test that uses random input to test the entire system
 */

#include"Test.h"
#include"../globals.h"
#include"MockQuestionAnswerCatalog.h"
#include"GoodTestInput.h"
#include"../DefaultParser.h"
#include"../DefaultProcessor.h"

namespace{
	constexpr std::size_t TRIES = 10000;
}

TEST(IT_GOOD_RANDOM_INPUT) {
	cgm::GoodTestInput input_provider{TRIES};
	cgm::DefaultParser input_parser;
	cgm::DefaultProcessor input_processor{ []( auto error_token ) {
		throw std::invalid_argument( std::format("Error: {}", error_token.second ) );
	} };

	cgm::Catalog catalog;

	cgm::MockQuestionAnswerCatalog{ catalog,input_provider, input_parser, input_processor }.execute();
	return {true, std::format("Completed {} inputs without error.", TRIES)};
} PASS;
