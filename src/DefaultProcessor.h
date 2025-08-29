#pragma once
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * DefaultProcessor implements the InputProcessor interface. This is the 
 * "normal" class that is used when the program is used as normal
 */

#include"InputProcessor.h"
#include"InputParser.h"

#include<functional>
namespace cgm {
	class DefaultProcessor : public InputProcessor{
		public:
		DefaultProcessor() noexcept;
		DefaultProcessor( std::function<void(decltype(*InputParser::ParserOutput{}.cbegin()))> f ) noexcept;
		virtual ~DefaultProcessor() noexcept override = default;

		virtual void process( InputParser::ParserOutput input, Catalog& catalog, std::ostream& output = std::cout ) override;

		private:
			std::function<void(decltype(*InputParser::ParserOutput{}.cbegin()))> _error_handler;
	};
}
