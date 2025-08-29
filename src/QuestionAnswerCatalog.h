/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * Facade to tie together the various (tiny) modules
 */

#include"InputProvider.h"
#include"InputParser.h"
#include"InputProcessor.h"

namespace cgm {
	class QuestionAnswerCatalog {
		public:
		QuestionAnswerCatalog( InputProvider& input_provider, InputParser& input_parser, InputProcessor& input_processor );
		
		void execute();

		private:
			InputProvider& _input;
			InputParser& _parser;
			InputProcessor& _processor;
	};
}
