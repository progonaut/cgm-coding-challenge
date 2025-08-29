#pragma once
/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * BadTestInput uses GoodTestInput to source good input, and then intersperses its own (bad) input.
 *
 * BadTestInput can indicate the expected result for the provided input
 */

#include"GoodTestInput.h"
#include"../InputProvider.h"

namespace cgm {
	class BadTestInput : public InputProvider {
		public:
			enum class Result {
				Pass,
				Fail,
				Exc
			};

		BadTestInput( std::size_t nr_of_inputs_before_eof ) noexcept;

			virtual std::string getLine( ) override;
			virtual bool eof( ) const noexcept override;

			virtual ~BadTestInput( ) override  = default;

			Result getResultFor( std::string input ) const noexcept;

		private:
			std::vector<std::pair<std::string,  Result>> _bad_input {
				{"Q1", Result::Fail},
				{"Q1? \"A", Result::Fail},
				{"Q1? \"A\" ?", Result::Fail},
				{"Q1", Result::Fail},
				{"Q1? \"A", Result::Fail},
				{"Q1? \"A\" ?", Result::Fail},
				{"Q1", Result::Fail},
				{"Q1? \"A", Result::Fail},
				{"Q1? \"A\" ?", Result::Fail},
			};

			GoodTestInput _good_input;

			std::size_t _max_nr_inputs, _provided_nr_inputs{0};
	};
}

		
