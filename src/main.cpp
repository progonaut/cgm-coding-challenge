/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * Task {

***
  1.  Write a C++ command line program that gives me two options. One to ask a specific question and the other option is
to add questions and their answers



Therefore the following restrictions apply:

  *   A Question is a String with max 255 chars
  *   An Answer is a String with max 255 chars
  *   A Question can have multiple answers (like bullet points)
  *   If the user asks a question it has to be exactly the same as entered – no “best match”.
  *   If the user asks a question which is not stored yet the program should print “the answer to life, universe and
everything is 42” according to “The hitchhikers guide to the Galaxy”
  *   If the user asks a question whish is  stored the program should print all answers to that question. Every Answer
in a separate line
  *   Adding a question looks like:
     *   <question>? “<answer1>” “<answer2>” “<answerX>”
     *   Char “?” is the separator between question and answers
     *   Every Question needs to have at least one answer but can have unlimited answers all inside of char “

  1.  Provide tests for the functionality described in 1)
  2.  Provide the source code on any GIT repo worldwide (GITLAB, GITHUB, whatever you prefer and/or use)
  3.  No other restrictions apply



Examples:

  *   Adding a question:
     *   What is Peters favorite food? “Pizza” “Spaghetti” “Ice cream”
  *   Asking a question which is in the system:
     *   What is Peters favorite food?
        *   Answers will be
           *   Pizza
           *   Spaghetti
           *   Ice cream
  *   Asking a question which is not in the system:
     *   When is Peters birthday?
        *   Answer will be
           *   the answer to life, universe and everything is 42

} ***
**/

#include <iostream>
#include <numeric>  //< for accumulate
#include <string>
#include <vector>
#include<format>

#include "globals.h"

#include "DefaultInput.h"
#include "DefaultParser.h"
#include "DefaultProcessor.h"
#include "QuestionAnswerCatalog.h"

#include"tests/TestRunner.h"

void usage( std::string program_name ) {
	// clang-format off
	std::cout
	  << "Usage: " << program_name << "[OPTIONS] [QUESTION [ANSWERS]]"
	  << "\n" << "Options"
	  << "\n" << "    -i, /i                                 Interactive mode; assumed if no command line parameters were given"
	  << "\n" << "    -v, /v                                 Sets mode to 'verbose', which may or may not print diagnostic information"
	  << "\n" << "    -?, /?                                 This screen."
	  << "\n" << "Questions (and answers)"
	  << "\n" << "    QUESTION                               Looks up a question and prints the associated answers"
	  << "\n" << "    QUESTION ANSWER ANSWER ...             Adds one or more answers to a question"
	  << "\n"
	  << "\n" << "Format (elide the surrounding ')"
	  << "\n" << "    QUESTION                                   'This is a question?'"
	  << "\n" << "    ANSWER                                     '\\\"This is an answer\\\"'" 
	  << std::endl;
	// clang-format on
}

int main( int argc, char **argv ) {
#if !defined( RUNTESTS )
	bool interactive{ argc == 1 ? true : false };  //< interactive mode automatic if there are no command line
												  //parameters;

	// Any parameters after the initial options is considered as a "normal" input
	std::string input_from_cmd_line;

	try {
		std::vector<std::string> cmd_line_parameters{ argv + 1, argv + argc };

		decltype( cmd_line_parameters )::const_iterator cmd_line_parameter_it;
		for( cmd_line_parameter_it = cmd_line_parameters.begin( ); cmd_line_parameter_it != cmd_line_parameters.end( );
		     cmd_line_parameter_it++ ) {
			auto const &parameter = *cmd_line_parameter_it;
			if( parameter.starts_with( "-" ) || parameter.starts_with( "/" ) ) {
				if( parameter.substr( 1 ) == "i" ) {
					interactive = true;
				} else if( parameter.substr( 1 ) == "v" ) {
					cgm::verbose = true;
				} else if( parameter.substr( 1 ) == "?" ) {
					usage( argv[0] );
					return 0;
				} else {
					throw std::invalid_argument( std::format( "Argument '{}' unkown.", parameter ) );
				}
			} else {
				break;
			}
		}


		if( cmd_line_parameter_it != cmd_line_parameters.end( ) ) {
			input_from_cmd_line = std::accumulate(
			  std::next( cmd_line_parameter_it ),
			  cmd_line_parameters.cend( ),
			  *cmd_line_parameter_it,
			  []( std::string acc, std::string next ) { return acc + " " + next; } );
		}

	} catch( std::exception const &e ) {
		std::cerr << "Error: " << e.what( ) << "\n";
		usage( argv[0] );
		return 1;
	}


	cgm::DefaultInput input_provider{interactive, input_from_cmd_line};
	cgm::DefaultParser input_parser;
	cgm::DefaultProcessor input_processor;

	cgm::QuestionAnswerCatalog{ input_provider, input_parser, input_processor }.execute();

#else
	if( argc == 2 ) {
		if( std::string{argv[1]} == "-v" ) {
			cgm::verbose = true;
		}
	}
	cgm::TestRunner::get().run();
#endif
}
