/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * IT-Bad is a simple integration test that uses random 
 * good input, interspersed with bad input, to test the entire system
 */

#include"Test.h"
#include"../util.h"
#include"../globals.h"
#include"MockQuestionAnswerCatalog.h"
#include"BadTestInput.h"
#include"../DefaultParser.h"
#include"../DefaultProcessor.h"

TEST(IT_BAD_RANDOM_INPUT) {
	cgm::BadTestInput input_provider{10000};
	cgm::DefaultParser input_parser;
	cgm::DefaultProcessor input_processor{ []( auto error_token ) {
		throw std::invalid_argument( cgm::util::buildString("Error: ", error_token.second ) );
	} };

	cgm::Catalog catalog;

	cgm::MockQuestionAnswerCatalog<cgm::BadTestInput>{ catalog,input_provider, input_parser, input_processor }.execute();
	return {true, cgm::util::buildString("Completed ", 10000, " inputs without error.")};
} NOEXCEPT
