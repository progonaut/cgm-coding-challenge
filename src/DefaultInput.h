#include"InputProvider.h"

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * Default input reads from std::cin if in interactive mode, and otherwise returns eof
 */

namespace cgm {
	class DefaultInput : public InputProvider {
		public:
			DefaultInput( bool interactive, std::string initial_input ) noexcept;
			virtual std::string getLine( ) override;
			virtual bool eof( ) const noexcept override;

			virtual ~DefaultInput( ) override  = default;

		private:
			bool _interactive;
			std::string _initial_input;
	};
}
