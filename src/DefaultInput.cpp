#include"DefaultInput.h"

#include<iostream>

namespace cgm {
	DefaultInput::DefaultInput( bool interactive, std::string initial_input ) noexcept 
	: _interactive{interactive}, _initial_input{initial_input} { }

	std::string DefaultInput::getLine() {
		std::string input;
		if( _initial_input.length() != 0 ) {
			std::swap(input, _initial_input);
			return input;
		} else if( _interactive ) {
			std::cout << "? ";
			std::getline(std::cin, input);
			return input;
		} else {
			return "";
		}
	}

	bool DefaultInput::eof() const noexcept {
		return (!_interactive && _initial_input.length() == 0 ) || std::cin.eof();
	}
}
