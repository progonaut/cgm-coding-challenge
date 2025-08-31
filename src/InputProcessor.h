#pragma once

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * InputProcessor is an interface to turn a list of tokens into action
 */

#include"InputParser.h"
#include"Catalog.h"
#include<iostream>

namespace cgm {
	class InputProcessor {
		public:
		virtual ~InputProcessor() noexcept = 0;


		virtual void process( InputParser::ParserOutput input, Catalog& catalog, std::ostream& output = std::cout  ) = 0;
	};
}
