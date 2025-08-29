#pragma once
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * InputProvider is a simple interface that provides line input
 */

#include <string>

namespace cgm {
class InputProvider {
  public:
	virtual ~InputProvider( ) noexcept         = 0;

	virtual std::string getLine( )     = 0;
	virtual bool eof( ) const noexcept = 0;
};
}  // namespace cgm
