#include"QuestionAnswerCatalog.h"

#include<string>
#include"Catalog.h"

#include<iostream>

namespace cgm {
	QuestionAnswerCatalog::QuestionAnswerCatalog( InputProvider& input_provider, InputParser& input_parser, InputProcessor& input_processor )
		: _input{ input_provider }
	, _parser{ input_parser }
	, _processor{ input_processor } {}


	void QuestionAnswerCatalog::execute() {
		cgm::Catalog catalog;

		while(!_input.eof()) {
			try {
				_processor.process( _parser.parse( _input.getLine()), catalog );
			} catch( std::length_error const& e ) {
				std::cerr << "Error: " << e.what() << std::endl;
			} catch( std::exception const& e ) {
				std::cerr << "An unrecoverable error has occurred. The program will now close." << std::endl;
				throw;
			}
		}
	}



}
