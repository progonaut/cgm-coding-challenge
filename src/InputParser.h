#pragma once
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * Interface for the "input parser", which does no attribution of parsed tokens
 *
 * The input parser has to
 * * parse input line-by-line
 * * assume that no state can be kept between lines (although it may do so internally)
 * * if the end-of-line is reached in an invalid state, the parser has to PREPEND an error token
 *   but can optionally provide the parsed tokens up to the error afterwards
 * * Normal parsing errors (i.e. unexpected end of line, et cetera) should not cause an exception
 *
 *
 * The input parser is expected to include "high level" tokens that provide context, i.e.
 * the input
 *     "This is a question?"
 * produces the following tokens in order:
 *     Query, Question
 *
 * while the input
 *     "This is a question? \"And this is one answer\" \"And this is another\""
 * produces
 *     Definition Question Answer Answer
 */

#include<utility>
#include<string>
#include<list>

namespace cgm {

	class InputParser {
		public:
	enum class ParserTokenType {
		Query,
		Definition,
		Question,
		Answer,
		Error
	};
		using ParserOutput = std::list<std::pair<ParserTokenType, std::string>>;
		virtual ~InputParser() noexcept = 0;

		virtual ParserOutput parse( std::string line ) = 0;	
	};
}
