#pragma once
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * GoodTestInput returns input from a list of "good" test input that should not cause any errors.
 * As any order of input ought to work, it randomly returns one of the lines
 */

#include"../InputProvider.h"
#include<vector>

namespace cgm {
	class GoodTestInput : public InputProvider {
		public:
			GoodTestInput( std::size_t nr_of_inputs_before_eof ) noexcept;
			virtual std::string getLine( ) override;
			virtual bool eof( ) const noexcept override;

			virtual ~GoodTestInput( ) override  = default;

		private:
			std::vector<std::string> _good_input {
				"Q1?",
				"Q1? Q2?",
				"Q1? Q2? Q3?",
				"Q1? \"Q1A1\"",
				"Q1? \"Q1A1\" \"Q1A2\"",
				"Q2?",
				"Q3?",
				"Q2? \"Q2A1\"",
				"Q2? \"Q2A2\"",
				"Q3? \"Q3A1\"",
				"Q3? \"Q3A1\" \"Q3A2\"",
			};

			std::size_t _max_nr_inputs, _provided_nr_inputs{0};
	};
}
