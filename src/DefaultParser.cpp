#include "DefaultParser.h"

#include <format>
#include <iostream>
#include <list>
#include <numeric>

namespace cgm {

DefaultParser::~DefaultParser( ) noexcept {}


/* The parser is a simple, hard-coded state machine
 *
 * It can
 * * read multiple "statements" from one line
 * * handle input errors
 * * returns a list of parsed and typed, but not attributed, tokens
 */
InputParser::ParserOutput DefaultParser::parse( std::string line ) {
	if( line.length( ) == 0 )
		return { };
	std::list<ParserOutput> output;

	enum Steps : std::size_t { Init = 0, Question = 10, QuestionDone = 11, Answer = 20, AnswerDone = 21 };

	std::size_t step{ Init };

	decltype( line.begin( ) ) pos_marker{ line.begin( ) };
	bool error{ false };

	auto newStatement = [&output]( ) {
		output.push_back( { } );
	};

	newStatement( );

	auto skipWs = []( auto &begin, auto end ) {
		while( begin != end && ( *begin == ' ' || *begin == '\t' ) ) {
			begin++;
		}
	};

	using PTT = InputParser::ParserTokenType;
	for( auto character = line.begin( ); character != line.end( ) && !error; ) {
		switch( step ) {
			case Init: {
				skipWs( character, line.end( ) );
				if( *character == '"' || *character == '?' ) {
					output.back( ).push_front(
					  { PTT::Error,
					    std::format(
					      "Unexpected input '{}' at index {}. Expected start of question.",
					      *character,
					      std::distance( line.begin( ), character ) ) } );
					error = true;
				} else {
					pos_marker = character;
					step       = Question;
				}
			} break;
			case Question: {
				if( *character == '?' ) {
					output.back( ).push_back( { PTT::Question, std::string{ pos_marker, character + 1 } } );

					pos_marker = character + 1;

					output.back( ).push_front( { PTT::Query, "" } );

					step = QuestionDone;
				} else if( *character == '"' ) {
					output.back( ).push_front(
					  { PTT::Error,
					    std::format(
					      "Unexpected input '{}' at index {} when parsing question. Expected '?'.",
					      *character,
					      std::distance( line.begin( ), character ) ) } );
					error = true;
				} else {
					// continue parsing a question until '?' or error
				}
			} break;
			case QuestionDone: {
				skipWs( character, line.end( ) );
				if( *character == '"' ) {
					// Question has pushed a Query token to the front earlier
					// this token is removed and replaced with a definition token
					output.back( ).pop_front( );

					output.back( ).push_front( { PTT::Definition, "" } );

					step = Answer;

					pos_marker = character + 1;
				} else {
					// A question following a different question

					newStatement( );

					step = Question;

					pos_marker = character;
					character--;  //< the character is automatically advanced at the end of the loop;
					              // stepping back one to not drop the first character of the next question
				}
			} break;
			case Answer: {
				if( *character == '"' ) {
					output.back( ).push_back( { PTT::Answer, std::string{ pos_marker, character } } );
					step = AnswerDone;
				} else {
					// in answer, anything but '"' is permissible and read
				}
			} break;
			case AnswerDone: {
				skipWs( character, line.end( ) );
				if( *character == '"' ) {
					step       = Answer;
					pos_marker = character + 1;
				} else {
					// any other character "resets" the state machine back to its initial state
					step = Init;
					character--;
					newStatement( );
				}
			}
		}
		character++;
	}

	// After parsing, the legal states are
	// AnswerDone
	// QuestionDone
	// Init
	while( output.size( ) != 0 && output.back( ).size( ) == 0 )
		output.pop_back( );
	if( error ) {
		if( output.size( ) > 1 ) {
			// The last statement had an error; this error will now be replicated at the front
			// if the erroneous statement is the first (only) statement, that is not necessary
			output.push_front( { output.back( ).front( ) } );
		}
	} else if( step != AnswerDone && step != QuestionDone && step != Init ) {
		// Parsing did stop in the middle; an error state is generated
		if( output.size( ) == 0 )
			newStatement( );
		if( step == Question ) {
			output.back( ).push_front(
			  { PTT::Error, "Unexpected end-of-input when parsing question. Expected '?'." });
		} else if( step == Answer ) {
			output.back( ).push_front(
			  { PTT::Error, "Unexpected end-of-input when parsing answer. Expected '\"'." } );
		} else {
			output.back( ).push_front(
			  { PTT::Error,
				std::format( "Unexpected end-of-input when parsing in step {}.", std::size_t(step) )});
		}
		if( output.size( ) > 1 ) {
			output.push_front( { output.back( ).front( ) } );
		}
	} else {
		// otherwise, parsing was successful and returns normally
	}

	return std::accumulate(
	  std::next( output.begin( ) ),
	  output.end( ),
	  *output.begin( ),
	  []( InputParser::ParserOutput acc, InputParser::ParserOutput const &current ) {
		  acc.insert( acc.end( ), current.begin( ), current.end( ) );
		  return acc;
	  } );
}
}  // namespace cgm
