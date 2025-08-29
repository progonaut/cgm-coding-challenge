#include "DefaultProcessor.h"

#include <iostream>

#include "util.h"

namespace cgm {
namespace {
	void processError( decltype( *InputParser::ParserOutput{ }.cbegin( ) )  error_token );
}

DefaultProcessor::DefaultProcessor( ) noexcept
: _error_handler{processError}{}


DefaultProcessor::DefaultProcessor( std::function<void(decltype(*InputParser::ParserOutput{}.cbegin()) )> f ) noexcept
: _error_handler{f}{ }

void DefaultProcessor::process( InputParser::ParserOutput input, Catalog &catalog ) {
	for( auto it = input.begin( ); it != input.end( ); ) {
		auto const &token = *it;

		switch( token.first ) {
			case InputParser::ParserTokenType::Error: {
				_error_handler( token );
				it = input.end( );  //< processError might throw in the future; for now, end the loop
			} break;
			case InputParser::ParserTokenType::Query: {
				auto expect_question = std::next( it );
				if(
				  expect_question == input.end( )
				  || expect_question->first != InputParser::ParserTokenType::Question ) {
					throw std::runtime_error( util::buildString( "Illegal state: QUERY without QUESTION." ) );
				}

				auto answers = catalog.getAnswersFor( Question{expect_question->second,{}} );
				for( auto const &a : answers ) {
					std::cout << "* " << a << "\n";
				}
				std::cout << std::flush;

				it = std::next( expect_question );
			} break;
			case InputParser::ParserTokenType::Definition: {
				auto expect_question = std::next( it );
				if(
				  expect_question == input.end( )
				  || expect_question->first != InputParser::ParserTokenType::Question ) {
					throw std::runtime_error( util::buildString( "Illegal state: DEFINITION without QUESTION." ) );
				}

				std::string question_text = expect_question->second;

				std::list<std::string> answers;
				auto answer_it = std::next( expect_question );

				while( answer_it != input.end( ) && answer_it->first == InputParser::ParserTokenType::Answer ) {
					answers.push_back( answer_it->second );
					answer_it = std::next( answer_it );
				}

				it = answer_it;

				auto [pos, newOrChanged, nr] = catalog.addQuestion(
				  std::move( question_text ), std::move( answers ), Catalog::MergeOrReplace::Merge );
#if defined( DEBUG )
				if( cgm::verbose ) {
					std::cout << ( newOrChanged == Catalog::NewOrChanged::New ? "Added " : "Modified " ) << "question '"
							  << pos->getQuestionText( ) << "'; " << nr << " new answers." << std::endl;
				}
#endif
			} break;
			default: {
				throw std::runtime_error(
				  util::buildString( "Illegal state with TYPE ", std::size_t(token.first) , "(", token.second, ")" ) );
			} break;
		}
	}
}

namespace {
	void processError( decltype( *InputParser::ParserOutput{ }.cbegin( ) ) error_token ) {
		std::cerr << "Illegal input: " << error_token.second << std::endl;
	}
}  // namespace
}  // namespace cgm
