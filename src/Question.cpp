#include "Question.h"


#include<format>

namespace cgm {
Question::Question( std::string question_text, AnswerSet answers, std::size_t max_size, bool allow_empty )
    : _question_text{ std::move( question_text ) }, _answers{ std::move( answers ) }, _max_size_of_text{ max_size } {
	if( !allow_empty  && _answers.size() == 0 ) {
		throw std::invalid_argument( std::format("Question '{}' has an empty answer set, but every question has to have at least one.", _question_text) );
	}

	checkLengthInvariant( );

	if( !_question_text.ends_with("?") ) {
		throw std::invalid_argument( std::format("Question '{} does not end with '?'.", _question_text ));
	}
}

void Question::checkLengthInvariant( ) const {
	if( _question_text.length( ) <= 1 ) {
		throw TooShortException{ " A question has to be at least 1 (one) character long, excluding the mandatory final "
		                         "'?'." };
	}

	if( _question_text.length( ) > _max_size_of_text ) {
		throw TooLongException(
				std::format(
		    " Including the mandatory final '?', a question may not exceed {} characters in length",
		    _max_size_of_text )
#if defined(DEBUG)
			+ " [" + _question_text + "]"
#endif
			);
	}

	for( auto const &answer : _answers ) {
		if( answer.length( ) == 0 ) {
			throw TooShortException{ " An answer has to be at least 1 (one) character long." };
		}

		if( answer.length( ) > _max_size_of_text ) {
			throw TooLongException(
			  std::format( " An answer may not exceed {} characters in length.", _max_size_of_text ) );
		}
	}
}

Question::AnswerSet const &Question::getAnswers( ) const noexcept {
	return _answers;
}

decltype(Question::AnswerAgnosticComparator) Question::AnswerAgnosticComparator;
}  // namespace cgm
