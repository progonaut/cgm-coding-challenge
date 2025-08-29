#include"InputParser.h"

namespace cgm {
	class DefaultParser : public InputParser {
		public:
			virtual ~DefaultParser() noexcept override;

			virtual InputParser::ParserOutput parse( std::string line ) override;
	};
}
