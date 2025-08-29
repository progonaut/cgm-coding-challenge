/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * IT-Good is a simple integration test that uses random input to test the entire system
 */

#include"Test.h"
#include"../util.h"
#include"../globals.h"
#include"MockQuestionAnswerCatalog.h"
#include"GoodTestInput.h"
#include"../DefaultParser.h"
#include"../DefaultProcessor.h"

TEST(IT_GOOD_RANDOM_INPUT) {
	cgm::GoodTestInput input_provider{10000};
	cgm::DefaultParser input_parser;
	cgm::DefaultProcessor input_processor{ []( auto error_token ) {
		throw std::invalid_argument( cgm::util::buildString("Error: ", error_token.second ) );
	} };

	cgm::Catalog catalog;

	cgm::MockQuestionAnswerCatalog{ catalog,input_provider, input_parser, input_processor }.execute();
	return {true, cgm::util::buildString("Completed ", 10000, " inputs without error.")};
} NOEXCEPT
