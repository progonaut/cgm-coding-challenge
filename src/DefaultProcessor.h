#pragma once
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * DefaultProcessor implements the InputProcessor interface. This is the 
 * "normal" class that is used when the program is used as normal
 */

#include"InputProcessor.h"
namespace cgm {
	class DefaultProcessor : public InputProcessor{
		public:
		DefaultProcessor();
		virtual ~DefaultProcessor() override;

		virtual void process( InputParser::ParserOutput input, Catalog& catalog ) override;

	};
}
