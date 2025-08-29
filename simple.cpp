
#include<iostream>
#include<string>
#include<map>
#include<vector>

int main() {

	std::map<std::string, std::vector<std::string>> questions;

	std::string input_line;
	std::cout << "Exit with Ctrl-D" << std::endl;

	std::cout << "? ";
	while( std::getline( std::cin, input_line ) ) {
		bool error{false};
		if( !input_line.contains("?")) {
			std::cerr << "Questions must end with '?'" << std::endl;
			error = true;
		} else {
			auto [first_part, second_part] = [input_line]() {
				return std::make_pair( input_line.substr(0, input_line.find("?")), input_line.substr(input_line.find("?")));
			}();

			if( first_part.length() > 255 ) {
				std::cerr << "Questions can not be longer than 255 characters." << std::endl;
			} else {
				std::vector<std::string> answers;

				auto begin = second_part.find("\"");
				do {
					if (begin != std::string::npos) {
						begin++;
						auto end = second_part.find("\"", begin);
						if (end != std::string::npos) {
							if( end - begin > 255 ) {
								std::cerr << "Answers can not be longer than 255 characters." << std::endl;
								error = true;
							} else {
								answers.push_back( second_part.substr(begin, end - begin));
								begin = second_part.find("\"", end + 1);
							}
						} else {
							std::cerr << "Answers must be surrounded by '\"'" << std::endl;
							error = true;
						}
					}
				} while( begin != std::string::npos && !error );

				if( error ) {
					std::cerr << "Aborting due to previous errors." << std::endl;
				} else {
					if( answers.size() >= 1 ) {
						questions[first_part] = answers;
					} else {
						if( questions.contains(first_part)) {
							for( auto const& ans : questions[first_part] ) {
								std::cout << "* " << ans << "\n";
							}

							std::cout << "[no further answers]" << std::endl;
						} else {
							std::cout << "the answer to life, universe and everything is 42" << std::endl;
						}
					}
				}
  			}
		}

		std::cout << "? ";
	}
}





